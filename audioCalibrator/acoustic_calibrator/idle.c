/* 
 *  Audio calibrator
 *  Filters an audio input with multi-channel bandpass gains determined by comparison of FFT
 *  
 *  Based on the idle example in CCS and code provided by David Romalo
 *  
 *  Designed by Alex Mueller & John Matson
 *  2020/11/18
 */
 
#define FILTER_SIZE 6//size of registers for the IIR filter
#define FFT_SIZE 16// size of registers for the FFT calculation
#define BAND_QUANTITY 4

#define xdc__strict//gets rid of #303-D typedef warning re Uint16, Uint32

#include <xdc/std.h>
//#include <xdc/runtime/Log.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Task.h>

#include "Peripheral_Headers/F2802x_Device.h"

//function prototypes:
extern void DeviceInit(void);

/* Flag used by idle function to check if interrupt occurred */
volatile Bool isrFlag = FALSE;

/* Counter incremented by timer interrupt */
volatile UInt tickCount = 0;

int16 newsample1;// sample from ADC_1
int16 newsample2;// sample from ADC_2
int16 reg_in[FILTER_SIZE];// buffer containing values to be filtered
int16 reg_out[FILTER_SIZE];// buffer containing filtered  values
int16 reg_fft_1[FFT_SIZE];// buffer for FFT 1
int16 reg_fft_2[FFT_SIZE];// buffer for FFT 2

int16 gain[BAND_QUANTITY];// buffer containing the gains of the 4 filters.

int16 k = 0;// circular buffer position variable
int16 i;// for loop iterator
int16 n;// local circular buffer variable
int16 ind;// buffer index variable

/*
 *  ======== main ========
 */
Int main()
{
    // test comment

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

/*
 *  ======== myTickFxn ========
 *  Timer Tick function that increments a counter, and sets the isrFlag.
 */
Void myTickFxn(UArg arg) 
{
    tickCount += 1;    /* increment the counter */

    isrFlag = TRUE;    /* tell background that new data is available */
}

/*
 *  ======== myIdleFxn ========
 *  Background idle function that is called repeatedly 
 *  from within BIOS_start() thread.
 */
Void myIdleFxn(Void) 
{
    if (isrFlag == TRUE) {
        isrFlag = FALSE;

    }
}


//HWI handler for the ADC results
void ADC_1(void) {
    //read ADC value
    AdcRegs.ADCSOCFRC1.all = 0x1; //start conversion via software
    while(AdcRegs.ADCINTFLG.bit.ADCINT1 == 0)
        {
        ; //wait for interrupt flag to be set
        }
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    newsample1 = AdcResult.ADCRESULT0; //get reading
}

void ADC_2(void) {
    //read ADC value
    AdcRegs.ADCSOCFRC1.all = 0x1; //start conversion via software
    while(AdcRegs.ADCINTFLG.bit.ADCINT1 == 0)
        {
        ; //wait for interrupt flag to be set
        }
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear interrupt flag

    newsample2 = AdcResult.ADCRESULT0; //get reading

}

//TSK handler calculates both FFTs, then modifies the gain
void fft(void) {

}

