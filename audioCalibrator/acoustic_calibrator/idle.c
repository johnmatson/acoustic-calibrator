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
#include <ti/sysbios/knl/Swi.h>
#include "fft.h"
#include "fft_hamming_Q31.h"
#include <math.h>

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

/* Flag used by idle function to check if interrupt occurred */
volatile Bool isrFlag = FALSE;

volatile int16 tickCount = 0;

int16 newsample1; // sample from ADC_1
int16 newsample2; // sample from ADC_2
int16 xn; // pre-filter input sample
int16 yn1, yn2, yn3, yn4; // post-filter pre-sum output samples
int16 yn; // post-filter post-sum output sample

#pragma DATA_SECTION(fftin1, "FFTipcbsrc");
int32 fftin1[FFT_SIZE]; // buffer for FFT 1
#pragma DATA_SECTION(fftout1, "FFTipcb");
int32 fftout1[FFT_SIZE]; // buffer for FFT 2
#pragma DATA_SECTION(fftmag, "FFTmagbuf");
int32 fftmag[FFT_SIZE];

int16 gain[BAND_QUANTITY]; // buffer containing the gains of the 4 filters.

int16 k = 0; // circular buffer position variable
int16 f = 0; // position variable for FFT filling
int16 i; // for loop iterator
int16 n; // local circular buffer variable
int16 ind; // buffer index variable
int16 fft_flag = 0; // bool used for fft buffer control

// Declare and initialize the structure object.
// Use the RFFT32_<n>P_DEFUALTS in the FFT header file if
// unsure as to what values to program the object with.
RFFT32  rfft = RFFT32_32P_DEFAULTS;

int16 count; // count for testing

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

    k = 0;

    DeviceInit(); // initialize peripherals
    BIOS_start();    /* does not return */
    return(0);
}

Void ADCtimer(UArg arg)
{
    AdcRegs.ADCSOCFRC1.all = 0x1; //start conversion via software
}

Void fftTimer(){
    isrFlag = TRUE;    /* tell background that new data is available */
}

/*
 *  ======== myIdleFxn ========
 *  This section performs the FFTs of the two inputs, then uses the result to modify the gain
 *
 */
Void myIdleFxn(Void) 
{

    if (fft_flag >= (FFT_SIZE) - 1) {
        count++;
        //int16 fft_out_2[FFT_SIZE*2];// output buffer for FFT 2

        //count++;
        RFFT32_brev(fftin1, fftout1, FFT_SIZE); // real FFT bit reversing

        rfft.ipcbptr = fftout1;                  // FFT computation buffer
        rfft.magptr  = fftmag;               // Magnitude output buffer
        //rfft.winptr  = (long *)win;           // Window coefficient array
        rfft.init(&rfft);                     // Twiddle factor pointer initialization

        rfft.calc(&rfft);                     // Compute the FFT
        rfft.split(&rfft);                    // Post processing to get the correct spectrum
        rfft.mag(&rfft);                      // Q31 format (abs(ipcbsrc)/2^16).^2

        fft_flag = 0;// reset the fft flag to allow the buffer to be refilled later
    }
}

// HWI handlers for the ADC results
// vector ID 32 is ADCINT1
Void ADC_1(Void) {
    int32 xn;
    //read ADC value
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    newsample1 = AdcResult.ADCRESULT0; //get reading
    xn = (newsample1 * 262144);
    if(fft_flag < (FFT_SIZE)) {
        fftin1[fft_flag] = xn;
        fft_flag++;
    }
}

// vector ID 33 is ADCINT2
Void ADC_2(Void) {
    //read ADC value
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear interrupt flag

    newsample2 = AdcResult.ADCRESULT1; //get reading
}

// SWI handler passes input signal 1 through four
// GEQ filters and sums output values with associated
// gains.
void filter(void) {
    // convert from shifted "unsigned" to Q1.15
    // by shifting to right align & flipping top bit
    xn = (newsample1 << 4) ^ 0x8000;

    iir1.input = xn;
    iir1.calc(&iir1);
    yn1 = iir1.output;

    iir2.input = xn;
    iir2.calc(&iir2);
    yn2 = iir2.output;

    iir3.input = xn;
    iir3.calc(&iir3);
    yn3 = iir3.output;

    iir4.input = xn;
    iir4.calc(&iir4);
    yn4 = iir4.output;

    // ADD SUMMING AND OUTPUT CONVERSION HERE
}
