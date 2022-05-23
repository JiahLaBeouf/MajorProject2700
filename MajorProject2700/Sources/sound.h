#ifndef __sound_h
#define __sound_h

// Choses next note to play
int noteSelector(char notes[8], char current_num);

// Delay to change frequency
void delay_function(char delay);

// Changes the list of notes
void update_notes(char notes[8]);

// Main function of sound module
void play_tune(int mode);

#endif
