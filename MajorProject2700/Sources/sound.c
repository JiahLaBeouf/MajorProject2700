/*
  Change mode variable to play different tune
*/

#include <hidef.h>     
#include "derivative.h"
#include "sound.h"
#define zero_in_ascii 48


// Defining functions
int noteSelector(char notes[8], char current_num);
void delay_function(char delay);
void update_notes(char notes[8]);
void play_tune(int mode);

int current_period;         // Period of note
char current_delay;         // How long note is held

// THIS VARIABLE SHOULD BE UPDATED USING A FUNCTION
// 1 for low inventory, 2 for normal (one item removed)
//int mode = 2;               


void play_tune(int mode) {   
    // String defining 4 notes in C major scale & length of each       
    char notes[8];      
    
    // Current note of string to play
    char current_num = 0; 
    
    // How many times played
    char current_line = 1;    

    
    // Setting up timer and interrupt
    
    // Disable Interrupts
    DisableInterrupts;
    TSCR1 = 0x90;             // Timer Enable and fast flag clear
    TSCR2 = 0x03;             // Set Prescaler to 8
    
    // Setting Up Output Compare Interrupt
    TIOS = 0x20;              // Set 5th Bit to Output Compare
    TIE = 0x20;               // Enable Interrupt for OC5
    TCTL1 = 0x04;             // Toggle OC5 upon successful OC
    TFLG1 = 0x20;             // Clear OC5

    // Enable Interrupts   
    EnableInterrupts;
    
  for(;;) {
    _FEED_COP(); /* feeds the dog */
    
    // Get notes to play
    update_notes(notes);
          
    // Selects frequency of note
    current_period = noteSelector(notes, current_num);
    
    // Set first output compare to happen
    TC5 = TCNT + current_period;
    
    // Identify desired delay
    current_delay = notes[current_num+4] - zero_in_ascii;
    
    // Implement Delay
    delay_function(current_delay);
    
    // Move on to next note
    current_num++;
    
    // Next line if applicable
    if (current_num > 3){
      current_num = 0;
      current_line++;
      
      if ((mode == 2) || (current_line>5)) {
        break;
      }
    }  
  }
  /* end of main */
}


/*
  Function Selects Note to Play
*/
int noteSelector(char notes[8], char current_num){
    char letters[8] = {'c','d','e','f','g','a','b','C'};
    int period[8] = {5733,5108,4551,4295,3827,3409,3037,2867};
    int i;     // iterator
    
    for (i = 0; i < 8; i++) {   
      if (notes[current_num] == letters[i]) {
        int current_period = period[i];

        TSCR1 = 0x90;           // Ensure timer enabled
        return current_period;
      }
    } 
    // if non a-g note, then silent
    TSCR1 = 0x10;               // Disable timer
    // if none, return C2 note (only for safety, shouldn't be heard)
    return 22632;  
}   


/* 
  Delay Function
  1 represents delay of approx 150 ms
  8 represents delay of approx 1200 ms
*/
void delay_function(char delay){
    int i, j;
    delay = 10*delay;
    for (i = 0; i < delay; i++) {
      for (j = 0; j < 13000; j++) {
          _FEED_COP();   
          // About 23 Cycles to increment
      }
    }
}


/*
   Function should update the notes currently played
*/
void update_notes(char notes[8]){  
  // Strings giving the tune to play
  char item_removed[8] = {'c','q','f','g','2','1','2','4'}; 
  char low_stock[8] = {'c','f','c','f','6','6','6','6'}; 
  char quiet[8] = {'q','q','q','q','1','1','1','1'};  
  int i;     // iterator
  
  if (mode == 1) {
    for (i = 0; i < 8; i++){
      notes[i] = low_stock[i];  
    }
  } 
  else if (mode == 2) {
    for (i = 0; i < 8; i++){
      notes[i] = item_removed[i];  
    }
  } 
  else {
   for (i = 0; i < 8; i++){
      notes[i] = quiet[i];  
    }   
  }
}
   
#pragma CODE_SEG __NEAR_SEG NON_BANKED
 
__interrupt void OC_interrupt(void) {
  
  // reset output compare to repeat
  TC5 = TC5 + current_period;   

}
