#ifndef ANIMATE_H
#define ANIMATE_H

#include "externs.h"
#include "enums.h"

void handleRun()
{
  if (RUN_DIR == 0) //Early bailout
  {
      return;
  }

  //What percent of crossfade time has elapsed?
  float percent = 0.0;
  
  if (RUN_MODE == RUN_FADE)
  {
    percent = (float)(TIME_NOW - TIMERS[TID_RUN]) / (float)RUN_XFADE_MS;

  }
  
  if (percent > 1.0f) percent=1.0f;
  if (percent < 0.0f) percent = 0.0f;
  
  COLOR ncol;

  for (int pos=0;pos<=NUM_LEDS;++pos) //Position in LEDs
  {
    int next = (pos + RUN_DIR + NUM_LEDS) % NUM_LEDS;  //Next position
    int out = (pos+RUN_START+NUM_LEDS)%NUM_LEDS; //Output position

    for (int i=0;i<4;++i)
    {

      int dist = abs(PIXELS[pos].c[i] - PIXELS[next].c[i]); //distance between cols
      unsigned char pval = round( (float)dist * percent ); //percentage value

      ncol.c[i] = PIXELS[next].c[i];  //Set to the next color.
      if ( PIXELS[pos].c[i] >= PIXELS[next].c[i]) //current color greater than next
      {
        ncol.c[i] += pval;
      }
      else
      {
        ncol.c[i] -= pval;
      }
    }
    H_LEDS.setPixelColor( out ,ncol.l);
  }
    
  if (RUN_MODE == RUN_ON)
  {
    percent = 0.0f;
    if ( times_up((long)RUN_ON_MS, TID_RUN) )
    {
      RUN_MODE = RUN_FADE;
    }
  }
  else
  {
    if (times_up((long)RUN_XFADE_MS, TID_RUN))
    {
      RUN_MODE = RUN_ON;
      RUN_START = (RUN_START + NUM_LEDS + RUN_DIR) % NUM_LEDS;
    }
  }
}

#endif //ANIMATE_H
