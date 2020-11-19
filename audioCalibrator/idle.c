/* 
 *  Audio calibrator
 *  Filters an audio input with multi-channel bandpass gains determined by comparison of FFT 
 *  
 *  Based on the idle example in CCS and code provided by David Romalo
 *  
 *  Designed by Alex Mueller & John Matson
 *  2020/11/18
 */
 
#define N//size of registers for the IIR filter

#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include "Peripheral_Headers/F2802x_Device.h"

//function prototypes:
extern void DeviceInit(void);

/* Flag used by idle function to check if interrupt occurred */
volatile Bool isrFlag = FALSE;

/* Counter incremented by timer interrupt */
volatile UInt tickCount = 0;

int16 new_sample1;
int16 new_sample2;
int16 reg_in1[N];
int16 reg_in2[N];
int16 reg_out[N];
int16 k = 0;
int16 i;
int16 n;

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
    DeviceInit(); //initialize peripherals
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
        /*
         * Print the current value of tickCount to a log buffer. 
         */
        //System_printf("Tick Count = %d\n", tickCount);

    }
}

void ADCin1(void) {
	new_sample1 = 0;
}

void ADCin2(void) {
    new_sample2 = 0;
}
