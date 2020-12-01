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

#include "Peripheral_Headers/F2802x_Device.h"

//function prototypes:
extern void DeviceInit(void);

/* Flag used by idle function to check if interrupt occurred */
volatile Bool isrFlag = FALSE;

int16 newsample1;// sample from ADC_1
int16 newsample2;// sample from ADC_2
int16 reg_in[FILTER_SIZE];// buffer containing values to be filtered
int16 reg_out[FILTER_SIZE];// buffer containing filtered  values

// Create the FFT buffers, ipcb and ipcbsrc, in sections with the same name
#pragma DATA_SECTION(ipcb, "FFTipcb");
int32_t ipcb[FFT_SIZE];

#pragma DATA_SECTION(ipcbsrc, "FFTipcbsrc");
int32_t ipcbsrc[FFT_SIZE];

#pragma DATA_SECTION(magnit, "FFTmagbuf");
int32_t magnit[FFT_SIZE];

// Declare and initialize the structure object.
// Use the RFFT32_<n>P_DEFUALTS in the FFT header file if
// unsure as to what values to program the object with.
RFFT32  rfft = RFFT32_32P_DEFAULTS;

int16 gain[BAND_QUANTITY];// buffer containing the gains of the filters.

int16 k;// circular buffer position variable
int16 i;// for loop iterator
int16 n;// local circular buffer variable
int16 ind;// buffer index variable
int16 fft_flag = 0;// integer used for fft buffer control

int16 count = 0;// count for testing

/*
 *  ======== main ========
 */
Int main()
{
    /* 
     * Start BIOS
     * Perform a few final initializations and then
     * fall into a loop that continually calls the
     * installed Idle functions.
     */

    //initialize arrays:
    for(i = 0; i < FILTER_SIZE; i++) {
        reg_in[i] = 0;
        reg_out[i] = 0;
    }

    for(i = 0; i < (FFT_SIZE); i++) {
        ipcbsrc[i] = 0;
        ipcb[i] = 0;
        magnit[i] = 0;

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
        RFFT32_brev(ipcbsrc, ipcb, FFT_SIZE); // real FFT bit reversing

        rfft.ipcbptr = ipcb;                  // FFT computation buffer
        rfft.magptr  = magnit;               // Magnitude output buffer
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
        ipcbsrc[fft_flag] = xn;
        fft_flag++;
    }
}

// vector ID 33 is ADCINT2
Void ADC_2(Void) {
    //read ADC value
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //clear interrupt flag

    newsample2 = AdcResult.ADCRESULT1; //get reading
}

Void filter(Void) {

}
