#include "derivative.h"      /* derivative-specific definitions */

#include "laser.h"

//The laser works by sending a PWM with a differing duty cycle based on how close an object is
// variables for reading the laser pulse timing
unsigned long riseEdge = 0; //The variable for the time at which the PWM pulse high starts
unsigned long fallEdge = 0; //The variable for the time at which the PWM pulse high ends
unsigned long lastLaserSample = 0; //The variable for the length of the PWM pulse (could be the variable for physical distance if calibrated and converted)

//This whole function basically turns on the timer, and sets it to input compare, meaning it records the time when something happens
//In this case the something is both the rising and falling edge of the PWM pulse, so the timer records when both happen
void laserInit(void){
                    
  TSCR1_TEN = 1;   // enable the timers (this might happen more than once)

  TIE_C1I = 1;     // enable interrupts for channel 1                         
  TIOS_IOS1 = 0;   // Set Laser ch1 to input capture
  
  TCTL4_EDG1A = 1;
  TCTL4_EDG1B = 1; // trigger on both rising and falling edge interrupts

  TFLG1 |= TFLG1_C1F_MASK; // Reset flag
}

//The function thats called from the main to get the most recent PWM signal length (most recent distance measured)
void GetLatestLaserSample(unsigned long *sample) {
  *sample = lastLaserSample;
}


//The code that triggers when the rising or falling edge is detected
#pragma CODE_SEG __NEAR_SEG NON_BANKED /* Interrupt section for this module. Placement will be in NON_BANKED area. */
__interrupt void TC1_ISR(void) { 
   
  // if captured a rising edge (lidar input is in port T bit 1)
  //Different bit for whether it was a rising or falling edge that triggered the interrupt, can assign variables based on that
  if (PTT_PTT1) { 
    riseEdge = (unsigned long)TC1; //Record rising edge time
  } 
  else {
    fallEdge = (unsigned long)TC1; //Record falling edge time

    if (fallEdge < riseEdge) {
        fallEdge = (unsigned long)65536 + (unsigned long)fallEdge; //If there was a clock overflow between capturing the rising and falling edge (falling edge has a lower value), adjust the value to compensate
    }

    lastLaserSample = (fallEdge - riseEdge)/24038; // Calculate the period of the PWM wave, should be in m. 1m = 1ms PWM and 24038 counts = 1ms
  
  TFLG1 |= TFLG1_C1F_MASK; // Reset flag
}
