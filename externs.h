#ifndef EXTERNS_H
#define EXTERNS_H

#include <Adafruit_NeoPixel.h>  //Someone else did the heavy lifting.  Say thanks!
#ifdef __AVR__
  #include <avr/power.h>
#endif

#include "color.h"
#include "rain.h"

extern const short NUM_LEDS; //Let's address this in V3!

extern Adafruit_NeoPixel H_LEDS;
extern SoftwareSerial BlueSerial;

extern COLOR PIXELS[];
extern Rain rain;

extern const short NUM_TIMERS;
extern unsigned long TIME_NOW; //The current time (in ms)
extern unsigned long TIMERS[ ];

extern const short MAX_BANDS;
extern COLOR BANDS[];
extern int BAND_WIDTH;
extern int NUM_BANDS;

extern char BUFFER[255]; /*Eat up a lot of memory for copying strings*/
extern bool FLASHLIGHT;
extern signed int MAXBRIGHT;
extern bool MODE_CHANGED;
extern int MODE_NEW;
extern unsigned short RAND_ON_MS;
extern signed short RUN_DIR;
extern unsigned short RUN_ON_MS;
extern unsigned short RUN_XFADE_MS;
extern int RUN_MODE;  //Displaying or fading
extern int RUN_START; //Where the run currently starts.

extern unsigned short SPARKLE_MS;
extern unsigned short SPARKLE_CHANCE;
extern int SPARKLE_SIZE;
extern COLOR SPARKLE_COLOR;

extern unsigned short BLINK[];
extern bool BLINKING;

bool times_up(unsigned long to_check, int timer_id);
void renderLEDS();
void initializeParameters();
unsigned char clamp(signed int val);
void handleRun();

#endif //EXTERNS_H
