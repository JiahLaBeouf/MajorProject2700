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
#include "accelerometer.h"
#include "gyro.h"
#include "iic.h"
#include "LCD.h"

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
  unsigned long lowestDistanceValue = 4294967295;
  int i;
  
  //Variables for calculating the number of milk cartons remaining & output
  unsigned long laserDistance;
  int storedPosition =0;
  unsigned long cartonLength = 0.098;
  int milkNum; 
  
  //Initialise magnet struct
  MagRaw read_magnet;
  
  //Turn on PWM and turn off cop
  PWMinitialise();
  laserInit();
  Init_LCD();
  iicSensorInit();
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
    //getRawDataMagnet(&read_magnet);
  
    //Get the latest laser reading
    GetLatestLaserSample(&laserDistance);
    
    //Store the highest mag value
    /*if(read_magnet.x>maxMagValue){
  
      //Store max value and position
      //maxMagValue = read_magnet.x;
      storedPosition = i;
    }*/
    
    //Store the cloest distance value
    if(laserDistance<lowestDistanceValue){
  
      //Store lowest distance and position
      lowestDistanceValue = laserDistance;
      storedPosition = i;
      
    }
    
    //Delay for 0.05sec
    delay();
  }
  
  //Set the servo to the position with the highest recorded value
  setHorizontal(storedPosition);
  
  
  /*MEASURING MILK CARTONS CODE*/
  GetLatestLaserSample(&laserDistance); //Obtain laser distance
  
  milkNum = round(5-((laserDistance-0.1)/cartonLength)); //Calculate the number of cartons remaining
  
  
  //Output string to LCD depending on number of cartons remaining
  if(milkNum == 5){
  
    writeStringLCD("There are 5 milk cartons remaining.");
  
  }else if(milkNum == 4){
  
    writeStringLCD("There are 4 milk cartons remaining.");
  
  }else if(milkNum == 3){
  
    writeStringLCD("There are 3 milk cartons remaining.");
  
  }else if(milkNum == 2){
  
    writeStringLCD("There are 2 milk cartons remaining.");
  
  }else if(milkNum == 1){
  
    writeStringLCD("There is 1 milk carton remaining. Restock!");
  
  }else if(milkNum < 1){
  
    writeStringLCD("There are 0 milk cartons remaining. Restock!");
  
  }else{
  
    writeStringLCD("Error: LIDAR too close.");
  
  }
  
 /*MEASURING WHICH MILK CARTON*/
  
  
  for(;;) {
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

