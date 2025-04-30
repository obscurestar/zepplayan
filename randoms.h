#ifndef RANDOMS_H
#define RANDOMS_H

#include "externs.h"
#include "enums.h"

COLOR getRandomColor()
{
  COLOR c;
  for (int i=0;i<4;++i)
  {
    c.c[i] = random(255);
  }
  return c;
}

void doRandom(bool all)
{
  //Don't change faster than we need to.
  if (!times_up((long)RAND_ON_MS, TID_RAND) )
  {
    return;
  }
  
  COLOR col = getRandomColor();
  for (int i=0;i<NUM_LEDS;++i)
  {
    if (!all)
    {
      col = getRandomColor();
    }
    H_LEDS.setPixelColor(i, col.l);
  }
  renderLEDS();
}

#endif //RANDOMS_H
