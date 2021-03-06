/* 
 *  Audio calibrator
 *  Filters an audio input with multi-channel bandpass gains determined by comparison of FFT
 *  
 *  Based on the idle example in CCS and code provided by David Romalo
 *  further code from TI c2000 examples
 *  
 *  Designed by Alex Mueller & John Matson
 *  2020/11/18
 */
 
// added by AM
#define FILTER_SIZE 6//size of registers for the IIR filter
#define FFT_SIZE 32// size of registers for the FFT calculation
#define BAND_QUANTITY 4


#define xdc__strict//gets rid of #303-D typedef warning re Uint16, Uint32

//modified by AM
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include "fft.h"
#include "fft_hamming_Q31.h"
#include <math.h>
#include <ti/sysbios/utils/Load.h>

// filter includes
//#include "device.h"
#include <filter.h>
#include "math.h"
#include "coeffs.h"

#include "Peripheral_Headers/F2802x_Device.h"

// create instances of IIR5BIQD16 module for each filter
// and place in "iirfilt" section
// added by JM
#pragma DATA_SECTION(iir1, "iirfilt");
#pragma DATA_SECTION(iir2, "iirfilt");
#pragma DATA_SECTION(iir3, "iirfilt");
#pragma DATA_SECTION(iir4, "iirfilt");

//added by JM
IIR5BIQ16 iir1 = IIR5BIQ16_DEFAULTS;
IIR5BIQ16 iir2 = IIR5BIQ16_DEFAULTS;
IIR5BIQ16 iir3 = IIR5BIQ16_DEFAULTS;
IIR5BIQ16 iir4 = IIR5BIQ16_DEFAULTS;
// create delay buffers for each filter and place in
// "iirldb" section
#pragma DATA_SECTION(dbuffer1, "iirldb");
#pragma DATA_SECTION(dbuffer2, "iirldb");
#pragma DATA_SECTION(dbuffer3, "iirldb");
#pragma DATA_SECTION(dbuffer4, "iirldb");
int16_t dbuffer1[2*IIR16_1_NBIQ];
int16_t dbuffer2[2*IIR16_2_NBIQ];
int16_t dbuffer3[2*IIR16_3_NBIQ];
int16_t dbuffer4[2*IIR16_4_NBIQ];

// added by JM
// create and populate coefficient variables for each filter
const int16_t coeff1[5*IIR16_1_NBIQ] = IIR16_1_COEFF;
const int16_t coeff2[5*IIR16_2_NBIQ] = IIR16_2_COEFF;
const int16_t coeff3[5*IIR16_3_NBIQ] = IIR16_3_COEFF;
const int16_t coeff4[5*IIR16_4_NBIQ] = IIR16_4_COEFF;

//function prototypes:
extern void DeviceInit(void);

//added by AM
int16 newsample1; // sample from ADC_1
int16 newsample2; // sample from ADC_2

//added by JM
int16 yn1, yn2, yn3, yn4; // post-filter pre-sum output samples
int16 yn; // post-filter post-sum output sample

//added by AM
#pragma DATA_SECTION(fftin1, "FFTipcbsrc");
#pragma DATA_SECTION(fftin2, "FFTipcbsrc");
int32 fftin1[FFT_SIZE]; // input buffer for FFT 1
int32 fftin2[FFT_SIZE]; // intput buffer for FFT 2
#pragma DATA_SECTION(fftout1, "FFTipcb");
#pragma DATA_SECTION(fftout2, "FFTipcb");
int32 fftout1[FFT_SIZE+2]; // output buffer for FFT 1
int32 fftout2[FFT_SIZE+2]; // output buffer for FFT 2
#pragma DATA_SECTION(magnitude1, "FFTmagbuf");
#pragma DATA_SECTION(magnitude2, "FFTmagbuf");
int32 magnitude1[FFT_SIZE];
int32 magnitude2[FFT_SIZE];

//added by JM
int16 gain[BAND_QUANTITY]; // buffer containing the gains of the 4 filters.
int32 gain32[BAND_QUANTITY];

// added by AM
int16 i; // for loop iterator
int16 fft_count = 0;// fft buffer loop counter
bool fft_flag = 0;// bool used for fft buffer control
int32 fft1sum[4]; // summing buckets for FFT1
int32 fft2sum[4]; // summing buckets for FFT2

// added by AM
Load_Stat stat;
Int CPULoad;
Int hwiLoad;
Int swiLoad;
Int taskLoad;

extern const Semaphore_Handle SEMFft;
extern const Swi_Handle SWIFilter;
extern const Swi_Handle SWIDac;
extern const Task_Handle ffthandle;

// Declare and initialize the structure object.
// added by AM
RFFT32  rfft = RFFT32_32P_DEFAULTS;

/*
 *  ======== main ========
 */
Int main()
{
    // filter initialization added by JM
    // initialize filter 1
    iir1.dbuffer_ptr  = (int16_t *)dbuffer1;
    iir1.coeff_ptr    = (int16_t *)coeff1;
    iir1.qfmat        = IIR16_1_QFMAT;
    iir1.nbiq         = IIR16_1_NBIQ;
    iir1.isf          = IIR16_1_ISF;
    iir1.init(&iir1);

    // initialize filter 2
    iir2.dbuffer_ptr  = (int16_t *)dbuffer2;
    iir2.coeff_ptr    = (int16_t *)coeff2;
    iir2.qfmat        = IIR16_2_QFMAT;
    iir2.nbiq         = IIR16_2_NBIQ;
    iir2.isf          = IIR16_2_ISF;
    iir2.init(&iir2);

    // initialize filter 3
    iir3.dbuffer_ptr  = (int16_t *)dbuffer3;
    iir3.coeff_ptr    = (int16_t *)coeff3;
    iir3.qfmat        = IIR16_3_QFMAT;
    iir3.nbiq         = IIR16_3_NBIQ;
    iir3.isf          = IIR16_3_ISF;
    iir3.init(&iir3);

    // initialize filter 4
    iir4.dbuffer_ptr  = (int16_t *)dbuffer4;
    iir4.coeff_ptr    = (int16_t *)coeff4;
    iir4.qfmat        = IIR16_4_QFMAT;
    iir4.nbiq         = IIR16_4_NBIQ;
    iir4.isf          = IIR16_4_ISF;
    iir4.init(&iir4);

    //initialize gain buffers to unity
    gain[0] = 1;
    gain[1] = 1;
    gain[2] = 1;
    gain[3] = 1;

    //for loops added by AM
    for(i = 0; i < (FFT_SIZE); i++) {
        fftin1[i] = 0;
        fftin2[i] = 0;
    }
    for(i = 0; i < (FFT_SIZE + 2); i++) {
        fftout1[i] = 0;
        fftout2[i] = 0;
    }

    /*
         * Start BIOS
         * Perform a few final initializations and then
         * fall into a loop that continually calls the
         * installed Idle functions.
         */

    DeviceInit(); // initialize peripherals
    BIOS_start();    /* does not return */
    return(0);
}

Void ADCtimer(UArg arg){ }// AM

/*
 *  ======== myIdleFxn ========
 *  This section performs the FFTs of the two inputs, then uses the result to modify the gain
 *
 */
Void myIdleFxn(Void) {// AM

    Load_getGlobalHwiLoad(&stat);               // HWI load check
    hwiLoad = Load_calculateLoad(&stat);

    Load_getGlobalSwiLoad(&stat);               // SWI load check
    swiLoad = Load_calculateLoad(&stat);

    Load_getTaskLoad(ffthandle, &stat);            // Task load check
    taskLoad = Load_calculateLoad(&stat);

    CPULoad = Load_getCPULoad();
    // Global CPU load check
    System_printf("CPU load = %d\nHWI load = %d\nSWI load = %d\nTASK load = %d\n", CPULoad, hwiLoad, swiLoad, taskLoad);
}

// HWI handlers for the ADC results
// vector ID 32 is ADCINT1
Void ADC_1(Void) { //AM
    //read ADC value
    newsample1 = (AdcResult.ADCRESULT0 << 4 ) ^ 0x8000; //get reading
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    Swi_post(SWIFilter);
}

// vector ID 33 is ADCINT2
Void ADC_2(Void) {// AM
    //read ADC value
    newsample2 = (AdcResult.ADCRESULT1 << 4 ) ^ 0x8000; //get reading
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear interrupt flag
}

// SWI handler passes input signal 1 through four
// GEQ filters and sums output values with associated
// gains.
void filter(void) { //JM
    // convert from shifted "unsigned" to Q1.15
    // by shifting to right align & flipping top bit

    iir1.input = newsample1;
    iir1.calc(&iir1);
    yn1 = iir1.output; // Q15

    iir2.input = newsample1;
    iir2.calc(&iir2);
    yn2 = iir2.output; // Q15

    iir3.input = newsample1;
    iir3.calc(&iir3);
    yn3 = iir3.output; // Q15

    iir4.input = newsample1;
    iir4.calc(&iir4);
    yn4 = iir4.output; // Q15

    // convert from Q15 to Q13
    yn1 = yn1 >> 2;
    yn2 = yn2 >> 2;
    yn3 = yn3 >> 2;
    yn4 = yn4 >> 2;

    // multiply & accumulate, possible gain values are limited to 1 through 7
    yn = (gain[0]*yn1) + (gain[1]*yn2) + (gain[2]*yn3) + (gain[3]*yn4); // Q13

    // convert from Q13 to Q11
    yn = (yn ^ 0x8000) >> 4; // Q11

    // convert from Q11 to "unsigned" value for output
    yn = yn & 0x8FFF;

    //block until Swi_post() added by AM
    if(~fft_flag && (fft_count < FFT_SIZE)) {
        fftin1[fft_count] = (int16)newsample1;
        fftin2[fft_count] = (int16)newsample2;
        fftin1[fft_count] = fftin1[fft_count] << 16;
        fftin2[fft_count] = fftin2[fft_count] << 16;

        fft_count++;
    }
    else {
        fft_count = 0;
        Semaphore_post(SEMFft);
    }

    Swi_post(SWIDac);
}


Void dac(Void) {// AM
    // write D-A output sample to SPI
    SpiaRegs.SPITXBUF = yn;
}

Void fft(Void) {

    while(TRUE) {

        // AM
        //=====================================
        Semaphore_pend(SEMFft, BIOS_WAIT_FOREVER);

        fft_flag = 1;//block filter function from updating fft buffer while calculations are being run

        //fft for input from ADC 1, reference signal
        RFFT32_brev(fftin1, fftout1, FFT_SIZE); // real FFT bit reversing
        rfft.ipcbptr = fftout1;                 // FFT computation buffer
        rfft.magptr  = magnitude1;                  // Magnitude output buffer
        rfft.init(&rfft);                       // Twiddle factor pointer initialization
        rfft.calc(&rfft);                       // Compute the FFT
        rfft.split(&rfft);                      // Post processing to get the correct spectrum
        rfft.mag(&rfft);                        // Q31 format (abs(ipcbsrc)/2^16).^2

        // sum 16 FFT bins into 4 bins for filter bands
        fft1sum[0] = magnitude1[0] + magnitude1[1] + magnitude1[2] + magnitude1[3];
        fft1sum[1] = magnitude1[4] + magnitude1[5] + magnitude1[6] + magnitude1[7];
        fft1sum[2] = magnitude1[8] + magnitude1[9] + magnitude1[10] + magnitude1[11];
        fft1sum[3] = magnitude1[12] + magnitude1[13] + magnitude1[14] + magnitude1[15];


        // fft for input from ADC 2, signal from 'microphone'
        RFFT32_brev(fftin2, fftout2, FFT_SIZE); // real FFT bit reversing
        rfft.ipcbptr = fftout2;                 // FFT computation buffer
        rfft.magptr  = magnitude2;                  // Magnitude output buffer
        rfft.init(&rfft);                       // Twiddle factor pointer initialization
        rfft.calc(&rfft);                       // Compute the FFT
        rfft.split(&rfft);                      // Post processing to get the correct spectrum
        rfft.mag(&rfft);                        // Q31 format (abs(ipcbsrc)/2^16).^2
        //======================================


        // JM
        // sum 16 FFT bins into 4 bins for filter bands
        fft2sum[0] = magnitude2[0] + magnitude2[1] + magnitude2[2] + magnitude2[3];
        fft2sum[1] = magnitude2[4] + magnitude2[5] + magnitude2[6] + magnitude2[7];
        fft2sum[2] = magnitude2[8] + magnitude2[9] + magnitude2[10] + magnitude2[11];
        fft2sum[3] = magnitude2[12] + magnitude2[13] + magnitude2[14] + magnitude2[15];


        // calculate actual/reference gain ratios
        gain32[0] = magnitude2[0] / (magnitude1[0] >> 2);
        gain32[1] = magnitude2[1] / (magnitude1[1] >> 2);
        gain32[2] = magnitude2[2] / (magnitude1[2] >> 2);
        gain32[3] = magnitude2[3] / (magnitude1[3] >> 2);

        // limit gain values to 1 through 7 
        if(gain32[0] < 1)
            gain[0] = 1;
        else if(gain32[0] > 7)
            gain[0] = 7;
        else
            gain[0] = gain32[0];
        
        if(gain32[1] < 1)
            gain[1] = 1;
        else if(gain32[1] > 7)
            gain[1] = 7;
        else
            gain[1] = gain32[1];
        
        if(gain32[2] < 1)
            gain[2] = 1;
        else if(gain32[2] > 7)
            gain[2] = 7;
        else
            gain[2] = gain32[2];
        
        if(gain32[3] < 1)
            gain[3] = 1;
        else if(gain32[3] > 7)
            gain[3] = 7;
        else
            gain[3] = gain32[3];


        fft_flag = 0;
        }

}
