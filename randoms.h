#ifndef RANDOMS_H
#define RANDOMS_H

#include "externs.h"
#include "enums.h"

COLOR getRandomColor()
{
  COLOR c;
  do {
    for (int i=0;i<4;++i)
    {
      c.c[i] = (random(16) * 16) - 1;
    }
  } while (c.l == 0xFFFFFFFF);
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
