//Uses Arduino Pro Mini board designation
//WARNING 3.3V board installed!

#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>  //Someone else did the heavy lifting.  Say thanks!
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "enums.h"
#include "blue_ui.h"
#include "rain.h"
#include "randoms.h"

//WARNING!!!!  NEOPIXEL RGB is 1,2,3 bytes of long, not 0.
//AND REVERSE ORDER

//Times are in miliseconds.  60000ms to the minute.
//const unsigned long gTimeToCooldown = 1000l * 3l;        //3 sec

const short PIN_LED = 6;  //Pin connecting the IN on the LED strip to the CPU board.
const short NUM_LEDS = 54; //Num LEDS in our array.
const short MAX_BANDS=12;  //Maximum number of color bands.

SoftwareSerial BlueSerial(4, 5); //TX, RX pins
Adafruit_NeoPixel H_LEDS = Adafruit_NeoPixel(NUM_LEDS, PIN_LED,NEO_RGB + NEO_KHZ800);

const short NUM_TIMERS = 4;
unsigned long TIME_NOW; //The current time (in ms)
unsigned long TIMERS[ NUM_TIMERS ];

COLOR PIXELS[NUM_LEDS]; //Duplicate of array used by H_LEDS
char BUFFER[255]; /*Eat up a lot of memory for copying strings*/
COLOR BANDS[MAX_BANDS];
int NUM_BANDS=0;
int BAND_WIDTH=2;
int MODE = CMD_COLOR;
int MODE_NEW = MODE;
signed short RUN_DIR=0;     //Direction of scrolling.
unsigned short RUN_ON_MS=500;  //On for 
unsigned short RUN_XFADE_MS=500; //Crossfade duration.
unsigned short SPARKLE_MS=500;  //Time between sparkles.
unsigned short SPARKLE_CHANCE=0; //No sparkles
int BLINK_STATE = BLINK_ON;
unsigned short BLINK[4];

bool BLINKING=false;

COLOR SPARKLE_COLOR;
int RUN_MODE=RUN_ON;  //Displaying or fading
int RUN_START=0; //Where the run currently starts.

bool MODE_CHANGED=true;
bool FLASHLIGHT = false;
bool DISPLAYED = false;   //Set true if display has run this time around.
unsigned short RAND_ON_MS=10; //Time to wait between steps in a display

//Initialize pattern classes
Rain rain;

//Pollute the global namespace!
signed int MAXBRIGHT = 2;      //Maximum brightness.

bool times_up(unsigned long to_check, int timer_id)
{
  if (MODE_CHANGED || (TIME_NOW - TIMERS[timer_id]) > to_check)
  {
    TIMERS[timer_id] = TIME_NOW;

    return true;
  }
  return false;
}

unsigned char clamp(signed int val)  //make int range 0-255
{
  unsigned char result=val;
  if (val > 255) result = 255;
  else if (val < 0) result = 0;
  return result;
}

void trimBrightness()
{
  for (int p=0;p<NUM_LEDS; ++p)  //Loop through pixels.
  {
    COLOR pc;
    pc.l = H_LEDS.getPixelColor(p);

    for (int c=0;c<4;++c)  //Loop through RBGW sub-pixels of each.
    {
      int icol = pc.c[c] * MAXBRIGHT;
      float fcol = (float) ((float)icol / 100.0f);
      icol = round( fcol );
      pc.c[c] = (unsigned char)icol;
    }

    H_LEDS.setPixelColor(p,pc.l);
  }
}

void handleRun()
{
  if (RUN_DIR == 0) //Early bailout
  {
      return;
  }

  if (RUN_MODE == RUN_ON)
  {
    for (int pos=0;pos<NUM_LEDS;++pos)
    {
      H_LEDS.setPixelColor( (pos+RUN_START+NUM_LEDS)%NUM_LEDS ,PIXELS[pos].l);
    }
    
    if ( times_up((long)RUN_ON_MS, TID_RUN) )
    {
      RUN_START = (RUN_START + NUM_LEDS + RUN_DIR) % NUM_LEDS;
      RUN_MODE = RUN_FADE;
    }

    return;
  }

  float percent = (float)(TIME_NOW - TIMERS[TID_RUN]) / (float)RUN_XFADE_MS;

  COLOR ncol;
  
  for (int led=0;led<=NUM_LEDS;++led)
  {
    int pos = led % NUM_LEDS;
    int next = (pos + RUN_DIR + NUM_LEDS) % NUM_LEDS;
    int out = (pos+RUN_START+NUM_LEDS)%NUM_LEDS;

    for (int i=0;i<4;++i)
    {
      int dist = abs(PIXELS[pos].c[i] - PIXELS[next].c[i]);
      unsigned char pval = round( (float)dist * percent );

      ncol.c[i] = PIXELS[pos].c[i];
      if ( ncol.c[i] <= PIXELS[next].c[i])
      {
        ncol.c[i] += pval;
      }
      else
      {
        ncol.c[i] -= pval;
      }
      clamp(ncol.c[i]); //waste cycles for paranoia
    }
    H_LEDS.setPixelColor( out ,ncol.l);
  }
  
  if (times_up((long)RUN_XFADE_MS, TID_RUN))
  {
    RUN_MODE = RUN_ON;
  }
}

void doSparkle()
{
  //Don't change faster than we need to.
  if (!times_up((long)SPARKLE_MS, TID_SPARKLE) )
  {
    return;
  }
  for (int i=0;i<NUM_LEDS;++i)
  {
    if (random(100) < SPARKLE_CHANCE)
    {
      H_LEDS.setPixelColor( i, SPARKLE_COLOR.l);
    }
  }
}

void doBlink()
{
  if (!BLINKING)
  {
    return;
  }
 
  float percent = 1.0f;
  COLOR color;
  
  if (BLINK_STATE != BLINK_ON)
  {
    if (BLINK_STATE == BLINK_OFF)
    {
      percent = 0;
    }
    else
    {
      if (BLINK[BLINK_STATE] > 0)
      {
        percent = (float)(TIME_NOW - TIMERS[TID_BLINK])/(float)BLINK[BLINK_STATE];
        if (percent>1.0f)
        {
          percent = 1.0f;
        }
      }

      if (BLINK_STATE==BLINK_OUT)
      {
        percent = 1.0f - percent;
      }
    }

    for (int i=0;i<NUM_LEDS;++i)
    {
      color.l = H_LEDS.getPixelColor(i);
      for(int j=0;j<4;++j)
      {
        color.c[j] = (unsigned char)round((float)color.c[j] * percent);
      }
      H_LEDS.setPixelColor(i, color.l);
    }
  }
  if (times_up(BLINK[BLINK_STATE], TID_BLINK))
  {
    BLINK_STATE = (BLINK_STATE+1) % 4;
  }
}

void renderLEDS()
{
  if (DISPLAYED)
  {
    return;
  }
  DISPLAYED = true;
  LEDStoPixels();
  handleRun();
  if (MODE != CMD_RAIN)
  {
    trimBrightness();
  }
  doBlink();
  doSparkle();
  H_LEDS.show();
  pixelsToLEDS();
}

void LEDStoPixels()
{ //Copy pixel values into LEDs
  for (int i=0;i<NUM_LEDS;++i)
  {
    PIXELS[i].l = H_LEDS.getPixelColor(i);  
  }
}

void pixelsToLEDS()
{ //Copy LED values into pixels.
  for (int i=0;i<NUM_LEDS;++i)
  {
    H_LEDS.setPixelColor(i, PIXELS[i].l);
  }
}

void setColor(int id, long color)
{
  if (id < NUM_LEDS)
  {
    H_LEDS.setPixelColor(id, color);
  }
}

void setColors(long color)
{
  for (int i=0;i<NUM_LEDS;++i)
  {
    setColor(i, color);
  }
}

void handleModeChange()
{
  if (MODE_CHANGED)
  {
    switch (MODE_NEW)
    {
      case CMD_FLASHLIGHT:
        if (FLASHLIGHT)
        {
          LEDStoPixels(); //Freeze the current state.
          setColors( 0xFFFFFFFF );
        }
        else
        {
          pixelsToLEDS(); //Thaw last state
        }
        H_LEDS.show();
        break;
      case CMD_RUN:
        renderLEDS();
        break;
      case CMD_RAIN:
        for(int i=0;i<NUM_LEDS;++i)
        {
          H_LEDS.setPixelColor(i,0);
        }
        MODE = MODE_NEW;
        break;
      case CMD_MAXBRIGHT:
        renderLEDS();
        break;
      case CMD_SPARKLE:
      case CMD_BLINK:
        break;
      default:
        MODE = MODE_NEW;
        break;
    }
  }
}

void makeStripes()
{
  int num=0;
  int i;
  for(i=0;i<NUM_BANDS;++i)
  {
    for(int j=0;j<BAND_WIDTH;++j)
    {
      H_LEDS.setPixelColor(num, BANDS[i].l);
      num++;
    }
  }
  for(i=num;i<NUM_LEDS;++i)
  {
    H_LEDS.setPixelColor(i, 0);
  }
  renderLEDS();
}

void doLighting()
{
  if (!FLASHLIGHT)
  {
    switch(MODE)
    {
      case CMD_RUN:

        renderLEDS();
        break;
      case CMD_COLOR:
        if( MODE_CHANGED )
        {
          setColors(PIXELS[0].l);
          renderLEDS();
        }
        break;
      case CMD_RAIN:
        if (!times_up((long)RAND_ON_MS, TID_RAND) )
        {
          return;
        }
        rain.mMaxBrightness=MAXBRIGHT;
        rain.loopStep();
        renderLEDS();
        break;
      case CMD_LIST:
      case CMD_PRIDE:
      case CMD_WIDTH:
        makeStripes();
        break;
      case CMD_RND:
        doRandom(true);
        break;
      case CMD_CHAOS:
        doRandom(false);
        break;
      default:
        break;
    }
    if (!DISPLAYED && (SPARKLE_CHANCE > 0 || BLINKING))
    {
      renderLEDS();
    }
  }
}

void setup() {
  BlueSerial.begin(9600);

  PIXELS[0].l = 0x007000FF;
  randomSeed(analogRead(0));  //Seed randomizer by grabbing whatever noise is on analog pin 0 at the moment.

  H_LEDS.begin(); //Initialize communication with LED array.
  H_LEDS.show(); //No values are set so this should be black.
  rain.mShiftOdds = 200;
  TIME_NOW=0;
  for (int i=0;i<NUM_TIMERS;++i)
  {
    TIMERS[i] = 0;
  }
  SPARKLE_COLOR.l = 0;

  showHelp();
}

void loop() {
  TIME_NOW = millis();
  handleInput();
  handleModeChange(); 
  doLighting();
  MODE_CHANGED = false;
  MODE_NEW = -1;
  DISPLAYED = false;
}
