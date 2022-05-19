#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "motorMovement.h" /*Moving the motor functions*/
#include "pll.h"
#include "l3g4200d.h"
#include "laser.h"
#include "magnetometer.h"

void delay(void)
{
  int j;
  for(j=1;j<=1201923;j++){
  }
        
}

void main(void) {
  /* put your own code here */
  
  
  /*IDENTIFYING MAGNET CODE*/
  //Variable storing the largest magnet value corrosponding towards pointing towards the magnet & position of servo
  int maxMagValue = 0;
  int maxPosition =0;
  int i;
  
  unsigned long laserDistance;
  unsigned long cartonLength = 0.098;
  int milkNum;
  //Initialise magnet struct
  MagRaw read_magnet;
  
  //Turn on PWM and turn off cop
  PWMinitialise;
  laserInit();
  EnableInterrupts;
  _DISABLE_COP();
   
  //Set the motors back to starting position
  setHorizontal(0);
  setVertical(90);
  
  //A for loop that cycles through 180 degrees slowly, recording the highest magnetometer value
  for(i=1;i<=180;i++){
  
    //Increment the horizontal angle by 1 degree 
    setHorizontal(i);
  
    //Read the magnet data
    getRawDataMagnet(&read_magnet);
  
    if(read_magnet.x>maxMagValue){
  
      //Store t
      maxMagValue = read_magnet.x;
      maxPosition = i;
    }
  
    //Delay for 0.05sec
    delay();
  }
  
  //Set the servo to the position with the highest recorded value
  setHorizontal(maxPosition);
  
  
  /*MEASURING MILK CARTONS CODE*/
  GetLatestLaserSample(&laserDistance);
  

  
  milkNum = round((laserDistance-0.01)/cartonLength);
  
  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

