#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include "derivative.h"      /* derivative-specific definitions */

// need this for string functions
#include <stdio.h>
#include <math.h>

#include "pll.h"
#include "simple_serial.h"

#include "l3g4200d.h"

#include "servo.h"
#include "laser.h"
#include "accelerometer.h"

#include "magnetomiter.h"

#include "gyro.h"
#include "LCD.h"

                    


void printErrorCode(IIC_ERRORS error_code) {
  char buffer[128];  
  switch (error_code) {
    case NO_ERROR: 
      sprintf(buffer, "IIC: No error\r\n");
      break;
    
    case NO_RESPONSE: 
      sprintf(buffer, "IIC: No response\r\n");
      break;
    
    case NAK_RESPONSE:
      sprintf(buffer, "IIC: No acknowledge\r\n");
      break;
    
    case IIB_CLEAR_TIMEOUT:
      sprintf(buffer, "IIC: Timeout waiting for reply\r\n");
      break;
    
    case IIB_SET_TIMEOUT: 
      sprintf(buffer, "IIC: Timeout not set\r\n");
      break;
    
    case RECEIVE_TIMEOUT:
      sprintf(buffer, "IIC: Received timeout\r\n");
      break;
    
    case IIC_DATA_SIZE_TOO_SMALL:
      sprintf(buffer, "IIC: Data size incorrect\r\n");
      break;

    default:
      sprintf(buffer, "IIC: Unknown error\r\n");
      break;
  }
    
  SerialOutputString(buffer, &SCI1);
}

      //NEW CODE//

struct MSG_HEADER{
  int sentinel;
  char msg_type[8];
  unsigned int msg_size;
  unsigned int header_time;
  int end_sentinel;
};

struct MSG_GYRO{
  int sentinel;
  int rotation_x;
  int rotation_y;
  int rotation_z;
  unsigned int last_sample_time;
};

struct MSG_MAG{
  int sentinel;
  int magnetometer_x;
  int magnetometer_y;
  int magnetometer_z;
  unsigned int last_sample_time;
};


struct MSG_LIDAR{
  int sentinal;
  unsigned long lidar_data;
  int morespacefilling;
  int spacefilling;
};



struct MSG_MSG{
  int sentinal;
  char msg_type[8];
  char msg[8];
};
  

struct MSG_BUTTONS{
  int sentinel;
  unsigned char button_states;
  unsigned int last_press_time;
};


void SendGyroMsg(int rot_x, int rot_y, int rot_z) {
  struct MSG_HEADER gyro_header = {0xABCD, "gyro", 0, 0, 0xDCBA};
  struct MSG_GYRO gyro_message = {0x9876, 0, 0, 0};
                             
  gyro_header.msg_size = sizeof(struct MSG_GYRO);

  gyro_message.rotation_x = rot_x;
  gyro_message.rotation_y = rot_y;
  gyro_message.rotation_z = rot_z;
  
  SerialOutputBytes((char*)&gyro_header, sizeof(struct MSG_HEADER), &SCI1);  
  SerialOutputBytes((char*)&gyro_message, sizeof(struct MSG_GYRO), &SCI1);  
}       
  
void SendMagMsg(int mag_x, int mag_y, int mag_z) {
  struct MSG_HEADER mag_header = {0xABCD, "mag", 0, 0, 0xDCBA};
  struct MSG_MAG mag_message = {0x9876, 0, 0, 0};
                             
  mag_header.msg_size = sizeof(struct MSG_MAG);

  mag_message.magnetometer_x = mag_x;
  mag_message.magnetometer_y = mag_y;
  mag_message.magnetometer_z = mag_z;
  
  SerialOutputBytes((char*)&mag_header, sizeof(struct MSG_HEADER), &SCI1);  
  SerialOutputBytes((char*)&mag_message, sizeof(struct MSG_MAG), &SCI1);  
}                 

  
void SendLidarMsg(unsigned long lidar_in) {
  struct MSG_HEADER lidar_header = {0xABCD, "lidar", 0, 0, 0xDCBA};
  struct MSG_LIDAR lidar_message = {0x9876, 0, 0, 0};
                             
  lidar_header.msg_size = sizeof(struct MSG_LIDAR);

  lidar_message.lidar_data = lidar_in;
  
  SerialOutputBytes((char*)&lidar_header, sizeof(struct MSG_HEADER), &SCI1);  
  SerialOutputBytes((char*)&lidar_message, sizeof(struct MSG_LIDAR), &SCI1);  
}                 
 
         
void main(void) {

  int milkNum;
  unsigned long cartonLength = 0.098;
  unsigned long value;
  int convert = 0; 
  
  // Initialising gyro, lidar, mag and text variables
  
  
  
  AccelRaw read_accel;
  AccelScaled scaled_accel;

  GyroRaw read_gyro;
  MagRaw read_magnet;
  
  unsigned long singleSample;
  
  char buffer[128];  
  
  // Unit Testing 
  
  IIC_ERRORS error_code = NO_ERROR;
  
  assert(error_code != NO_ERROR);


  //// CHECK IF NEED THIS    ////
    
    
    
  #ifndef SIMULATION_TESTING

  // make sure the board is set to 24MHz
  //  this is needed only when not using the debugger
  PLL_Init();
  Init_LCD();

  // initialise PWM
  PWMinitialise();
  setServoPose(1, 1);

  #endif
  
  
  
  //// CAN CROP ^^^ ////
  
  
  
  // initialise the simple serial
  SerialInitialise(BAUD_9600, &SCI1);
  
  #ifndef SIMULATION_TESTING
  
  // initialise the sensor suite
  error_code = iicSensorInit();
  
  // write the result of the sensor initialisation to the serial
  if (error_code == NO_ERROR) {
    sprintf(buffer, "NO_ERROR\r\n");
    SerialOutputString(buffer, &SCI1);
  } else {
    sprintf(buffer, "ERROR %d\r\n");
    SerialOutputString(buffer, &SCI1);
  }

  laserInit();
  
  //#else
  
  #endif

  Init_TC6();
  
	EnableInterrupts;
  COPCTL = 7;
  _DISABLE_COP();
    
  for(;;) {
  
    //#ifndef SIMULATION_TESTING
  
    // read the raw values
    error_code = getRawDataGyro(&read_gyro);   
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
       
      error_code = iicSensorInit();
      printErrorCode(error_code);   
    }
    
    error_code = getRawDataAccel(&read_accel);
    if (error_code != NO_ERROR) {
      printErrorCode(error_code);   
    
      error_code = iicSensorInit();
      printErrorCode(error_code); 
    }
    
    error_code = getRawDataMagnet(&read_magnet);
    
    getRawDataMagnet(&read_magnet); 
    
    GetLatestLaserSample(&singleSample);
    
    //// SIMULATION TESTING CODE ////
        
    //#else
    
    // inject some values for simulation
    //read_gyro.x = 123; read_gyro.y = 313; read_gyro.z = 1002;
    //read_accel.x = 124; read_accel.y = 312; read_accel.z = 2002;
    //read_magnet.x = 125; read_magnet.y = 311; read_magnet.z = 3002;
    //read_lidar = 1000;
    
    ////    end testing code    ////
    
    //#endif

    // convert the acceleration to a scaled value
    convertUnits(&read_accel, &scaled_accel);    
    
    // format the string of the sensor data to go the the serial    
    sprintf(buffer, "%lu, %d, %d, %d, %.2f, %.2f, %.2f\r\n", singleSample, read_gyro.x, read_gyro.y, read_gyro.z, scaled_accel.x, scaled_accel.y, scaled_accel.z);
    
    
    // output the data to serial
    //SerialOutputString(buffer, &SCI1);
    //SendGyroMsg(read_gyro.x, read_gyro.y, read_gyro.z);
    SendMagMsg(read_magnet.x, read_magnet.y, read_magnet.z);
    //SendLidarMsg(singleSample);
    
    value = conversion();
    
    convert = (value)/24038; // Calculate the period of the PWM wave, should be in m. 1m = 1ms PWM and 24038 counts = 1ms
    
    
    // Output code //       
    milkNum = floor(5-((convert-0.1)/cartonLength)); //Calculate the number of cartons remaining
  
  
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
    
    /*
    // Sound Effects
    if (milkNum > 1){
      play_tune(2);
    }
    else {
      play_tune(1);
    }
    */
      
    
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  
  /* please make sure that you never leave main */
}
