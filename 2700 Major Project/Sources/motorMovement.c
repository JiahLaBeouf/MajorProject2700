#include "derivative.h"
#include <math.h> 
#include "motorMovement.h"


#define ZERO_ELEVATION_DUTY 4600
#define ZERO_AZIMUTH_DUTY 2000


/***INITIALISE MOTOR***/
void PWMinitialise(void){
    // set PP5 and PP7 for pwm output 
    PWMCLK = 0; // select clock A
    PWMPOL = 0xA0; // PWM5 and PWM7 output start high
    PWMCTL = 0xC0; // 16-bit PWM: use PWM4 and PWM5 for PWM5, PWM6 and PWM7 for PWM7
    PWMCAE = 0; // Left aligned (not centre)
    PWMPRCLK = 0x33; // PWM Clock prescaler to 8 

    // set the PWM period appropriate for servos (60010 counts)
    PWMPER45 = 0xEA6A;
    PWMPER67 = 0xEA6A;

    // set the initial duty cycle for both servos
    PWMDTY45 = 90; //(90 = neutral position)
    PWMDTY67 = 90; //(90 = neutral position)
    PWME  |= 0xFF; // enable PWM0
}


/***SET HORIZONTAL ANGLE***/
void setHorizontal(int hAngle){

      
    PWMDTY67 = (int)(20*hAngle + 2700); //Uses formula to convert desired angle to appropriate duty cycle
}


/***SET VERTICAL ANGLE***/
void setVertical(int vAngle){
 
     PWMDTY45 = (int)(20*vAngle + 2700); //Uses formula to convert desired angle to appropriate duty cycle

}
