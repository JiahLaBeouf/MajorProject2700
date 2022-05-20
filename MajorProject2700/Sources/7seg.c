#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

/*
  To use, just replace the two lines in main function labelled
*/



/* General Time Delay Function */
void segDelay() {
   int c, d; 
   // For some reasons fks up if just use 15000 in one loop so eh 
   for (c = 1; c <= 50; c++) {
       for (d = 1; d <= 3000; d++) 
       {} 
   }
}

/* Updates the 7-seg PORT depending on number of cartons */
void update_port(int number_of_cartons) {
  short seven_seg[] = 
  {
          0x3F,   // 0
          0x06,   // 1
          0x5B,   // 2
          0x4F,  
          0x66,  
          0x6D,  
          0x7D,  
          0x07,   
          0x7F,   
          0x6F,   
          0x77,   
          0x7C,   // ... 
          0x39,   
          0x5E,   
          0x79,   // E
          0x71    // F 
  };
  if(number_of_cartons < 16) {
    PORTB = seven_seg[number_of_cartons];
  } 
  else {
    PORTB = seven_seg[0];
  }
}



void main(void) {

  int number_of_cartons = 3;      // REPLACE W ORIGINAL QUANTITY

  
  // Enable PORTB and PORTP
  DDRB = 0b11111111;
  DDRP = 0b00111111;
  
  PTP = 0b11111110;
  update_port(number_of_cartons);

	EnableInterrupts;

  for(;;) {
    segDelay();
    
    number_of_cartons = number_of_cartons + 1; // REPLACE WITH UPDATE FUNCTION
    
    if(number_of_cartons >= 16) {               // TO REMOVE AFTER
      number_of_cartons = number_of_cartons - 16;
    }
    
    update_port(number_of_cartons);

    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
           
