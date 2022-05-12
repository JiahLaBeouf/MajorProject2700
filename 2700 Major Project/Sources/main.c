#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "motorMovement.h" /*Moving the motor functions*/



void main(void) {
  /* put your own code here */
  PWMinitialise;
  setHorizontal(90);
  setVertical(90);


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
