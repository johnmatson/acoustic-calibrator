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
 
#define FILTER_SIZE 6//size of registers for the IIR filter
#define FFT_SIZE 32// size of registers for the FFT calculation
#define BAND_QUANTITY 4
#define PI 3.14159265358979323


#define xdc__strict//gets rid of #303-D typedef warning re Uint16, Uint32

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Swi.h>
#include "fft.h"
#include "fft_hamming_Q31.h"
#include <math.h>
#include <ti/sysbios/utils/Load.h>    // AF CK

// filter includes
//#include "device.h"
#include <filter.h>
#include "math.h"
#include "coeffs.h"

#include "Peripheral_Headers/F2802x_Device.h"

// create instances of IIR5BIQD16 module for each filter
// and place in "iirfilt" section
#pragma DATA_SECTION(iir1, "iirfilt");
#pragma DATA_SECTION(iir2, "iirfilt");
#pragma DATA_SECTION(iir3, "iirfilt");
#pragma DATA_SECTION(iir4, "iirfilt");

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

// create and populate coefficient variables for each filter
const int16_t coeff1[5*IIR16_1_NBIQ] = IIR16_1_COEFF;
const int16_t coeff2[5*IIR16_2_NBIQ] = IIR16_2_COEFF;
const int16_t coeff3[5*IIR16_3_NBIQ] = IIR16_3_COEFF;
const int16_t coeff4[5*IIR16_4_NBIQ] = IIR16_4_COEFF;

//function prototypes:
extern void DeviceInit(void);

int16 newsample1; // sample from ADC_1
int16 newsample2; // sample from ADC_2
int16 yn1, yn2, yn3, yn4; // post-filter pre-sum output samples
int16 yn; // post-filter post-sum output sample

#pragma DATA_SECTION(fftin1, "FFTipcbsrc");
#pragma DATA_SECTION(fftin2, "FFTipcbsrc");
int32 fftin1[FFT_SIZE]; // input buffer for FFT 1
int32 fftin2[FFT_SIZE]; // intput buffer for FFT 2
#pragma DATA_SECTION(fftout1, "FFTipcb");
#pragma DATA_SECTION(fftout2, "FFTipcb");
int32 fftout1[FFT_SIZE]; // output buffer for FFT 1
int32 fftout2[FFT_SIZE]; // output buffer for FFT 2


int16 gain[BAND_QUANTITY]; // buffer containing the gains of the 4 filters.

int16 i; // for loop iterator
int16 fft_count = 0;// fft buffer loop counter
bool fft_flag = 0;// bool used for fft buffer control

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
// Use the RFFT32_<n>P_DEFUALTS in the FFT header file if
// unsure as to what values to program the object with.
RFFT32  rfft = RFFT32_32P_DEFAULTS;

int16 count = 0; // count for testing

/*
 *  ======== main ========
 */
Int main()
{
    
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

    /* 
     * Start BIOS
     * Perform a few final initializations and then
     * fall into a loop that continually calls the
     * installed Idle functions.
     */

    for(i = 0; i < (FFT_SIZE); i++) {
        fftin1[i] = 0;
        fftout1[i] = 0;
        fftmag[i] = 0;

        //fft_in_2[i] = 0;
    }

    DeviceInit(); // initialize peripherals
    BIOS_start();    /* does not return */
    return(0);
}

Void ADCtimer(UArg arg){

}

/*
 *  ======== myIdleFxn ========
 *  This section performs the FFTs of the two inputs, then uses the result to modify the gain
 *
 */
Void myIdleFxn(Void) {

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
Void ADC_1(Void) {
    //read ADC value
    newsample1 = (AdcResult.ADCRESULT0 << 4 ) ^ 0x8000; //get reading
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    Swi_post(SWIFilter);
}

// vector ID 33 is ADCINT2
Void ADC_2(Void) {
    //read ADC value
    newsample2 = (AdcResult.ADCRESULT1 << 4 ) ^ 0x8000; //get reading
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear interrupt flag
}

// SWI handler passes input signal 1 through four
// GEQ filters and sums output values with associated
// gains.
void filter(void) {
    // convert from shifted "unsigned" to Q1.15
    // by shifting to right align & flipping top bit

    iir1.input = newsample1;
    iir1.calc(&iir1);
    yn1 = iir1.output;

    iir2.input = newsample1;
    iir2.calc(&iir2);
    yn2 = iir2.output;

    iir3.input = newsample1;
    iir3.calc(&iir3);
    yn3 = iir3.output;

    iir4.input = newsample1;
    iir4.calc(&iir4);
    yn4 = iir4.output;

    // ADD SUMMING AND OUTPUT CONVERSION HERE

    yn = (gain[0]*yn1) + (gain[1]*yn2) + (gain[2]*yn3) + (gain[3]*yn4);


    if(~fft_flag && (fft_count < FFT_SIZE)) {
        fftin1[fft_count] = newsample1;
        fftin2[fft_count] = newsample2;
        fft_count++;
    }
    else {
        fft_count = 0;
        Semaphore_post(SEMFft);
    }

    Swi_post(SWIDac);
}

Void dac(Void) {
    SpiaRegs.SPITXBUF = (yn ^ 0x8000) >> 4;
}


Void fft(Void) {

    while(TRUE) {

        count++;
        Semaphore_pend(SEMFft, BIOS_WAIT_FOREVER);

        fft_flag = 1;//block filter function from updating fft buffer while calculations are being run


        //fft for input from ADC 1, reference signal
        RFFT32_brev(fftin1, fftout1, FFT_SIZE); // real FFT bit reversing
        rfft.ipcbptr = fftout1;                  // FFT computation buffer
        rfft.magptr  = fftin1;               // Magnitude output buffer
        rfft.init(&rfft);                     // Twiddle factor pointer initialization
        rfft.calc(&rfft);                     // Compute the FFT
        rfft.split(&rfft);                    // Post processing to get the correct spectrum
        rfft.mag(&rfft);                      // Q31 format (abs(ipcbsrc)/2^16).^2


        // fft for input from ADC 2, signal from 'microphone'
        RFFT32_brev(fftin2, fftout2, FFT_SIZE); // real FFT bit reversing
        rfft.ipcbptr = fftout2;                  // FFT computation buffer
        rfft.magptr  = fftin2;               // Magnitude output buffer
        rfft.init(&rfft);                     // Twiddle factor pointer initialization
        rfft.calc(&rfft);                     // Compute the FFT
        rfft.split(&rfft);                    // Post processing to get the correct spectrum
        rfft.mag(&rfft);                      // Q31 format (abs(ipcbsrc)/2^16).^2


        fft_flag = 0;

        }

}
