#include <hidef.h>      /* common defines and macros */
#include <assert.h>
#include <stdio.h>
#include "derivative.h"      /* derivative-specific definitions */

#include "motorMovement.h" /*Moving the motor functions*/
#include "pll.h"
#include "l3g4200d.h"
#include "laser.h"
#include "magnetometer.h"


void main(void) {
  /* put your own code here */
  
  MagRaw read_magnet;
  getRawDataMagnet(&read_magnet);
  
  PWMinitialise;
  setHorizontal(90);
  setVertical(90);


	EnableInterrupts;


  for(;;) {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
