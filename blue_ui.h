/*ui.h
 * Routines related to getting input from the user.
 * PLEASE NOTE:  This file is only included for the CMDR, not receivers.
 */
 
#ifndef BLUE_UI_H
#define BLUE_UI_H

#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

#include "enums.h"
#include "externs.h"
#include "color.h"

//  STATE MACHINE VARIABLES //////////////////////////////////////
char CMDBUF[32];
int  CMDBUFIDX=0;
int CMD = -1; //Command waiting for parameters.  -1 for none.
int PARAM= 0; //Which param we're waiting on for this command.
int PTMP = 0;

//  PROGMEM DATA  ////////////////////////////////////////////////
//Set up progmem.  This is awful and ugly.
const char CMD0[] PROGMEM = "DIGITALRAIN";
const char CMD1[] PROGMEM = "EPILE";
const char CMD2[] PROGMEM = "NUTS";
const char CMD3[] PROGMEM = "PRIDE";
const char CMD4[] PROGMEM = "COLOR";
const char CMD5[] PROGMEM = "LIST";
const char CMD6[] PROGMEM = "ANIMATE";
const char CMD7[] PROGMEM = "BLINK";
const char CMD8[] PROGMEM = "MAXBRIGHT";
const char CMD9[] PROGMEM = "FLASHLIGHT";
const char CMD10[] PROGMEM = "SPARKLE";
const char CMD11[] PROGMEM = "WIDTH";
const char CMD12[] PROGMEM = "HELP";
const char CMD13[] PROGMEM = "QUIT";

const char *const CMDS[] PROGMEM = { CMD0, CMD1, CMD2, CMD3, CMD4, CMD5, CMD6, CMD7, CMD8, CMD9, CMD10, CMD11, CMD12, CMD13 };

const char DESC0[] PROGMEM = "Digital Rain\n\t<decay_odds>Likelihood of increasing base color\n\t<shift_odds>Likelihood of changing base color.";
const char DESC1[] PROGMEM = "All a random color";
const char DESC2[] PROGMEM = "Each a random color";
const char DESC3[] PROGMEM = "Pride Rainbow";
const char DESC4[] PROGMEM = "All to a color";
const char DESC5[] PROGMEM = "List of colors";
const char DESC6[] PROGMEM = "Chase params for lists";
const char DESC7[] PROGMEM = "Strobe/fade params for all patterns";
const char DESC8[] PROGMEM = "Max brightness for all patterns";
const char DESC9[] PROGMEM = "Toggle flashlight on and off.";
const char DESC10[] PROGMEM = "Sparkle behavior for patterns.";
const char DESC11[] PROGMEM = "Change width of list bands";
const char DESC12[] PROGMEM = "Display this.\n\tCase-insensitive partial names allowed.";
const char DESC13[] PROGMEM = "QUIT current display, reset to default.";

const char *const DESCS[] PROGMEM = { DESC0, DESC1, DESC2, DESC3, DESC4, DESC5, DESC6, DESC7, DESC8, DESC9, DESC10, DESC11, DESC12, DESC13 };

const int MAX_CMDS = 13;

const char COL0[] PROGMEM = "RED";
const char COL1[] PROGMEM = "ORANGE";
const char COL2[] PROGMEM = "YELLOW";
const char COL3[] PROGMEM = "GREEN";
const char COL4[] PROGMEM = "BLUE";
const char COL5[] PROGMEM = "PURPLE";
const char COL6[] PROGMEM = "CYAN";
const char COL7[] PROGMEM = "MAGENTA";
const char COL8[] PROGMEM = "WHITE";

const char *const COLS[] PROGMEM = { COL0, COL1, COL2, COL3, COL4, COL5, COL6, COL7, COL8 };

const unsigned long COL_VALS[] PROGMEM = {
      0x00FF0000,   //red
      0x00FF6000,   //orange
      0x00FFFF00,   //yellow
      0x0000FF00,   //green
      0x000000FF,   //blue
      0x007000FF,   //purple
      0x0000FFFF,   //cyan
      0x00FF00FF,   //magenta
      0x00FFFFFF    //white
};

const int MAX_COLS = 8;

const char PARAM0[] PROGMEM = "ON ms: ";
const char PARAM1[] PROGMEM = "OFF ms: ";
const char PARAM2[] PROGMEM = "crossfade ms: ";
const char PARAM3[] PROGMEM = "fadeout ms: ";
const char PARAM4[] PROGMEM = "fadein ms: ";
const char PARAM5[] PROGMEM = "pixels per band: ";
const char PARAM6[] PROGMEM = "color (RGB 0 to oct 888 or name): ";
const char PARAM7[] PROGMEM = "percent brightness (0-100): ";
const char PARAM8[] PROGMEM = "percent chance per pixel(0-100): ";
const char PARAM9[] PROGMEM = "direction (-1 to 1): ";
const char PARAM10[] PROGMEM = "color (RGB 0 to oct 888 or name. x to end): ";
const char PARAM11[] PROGMEM = "frequency (ms): ";
const char PARAM12[] PROGMEM = "Decay Odds: (lower is faster) ";
const char PARAM13[] PROGMEM = "ON ms (x to end): ";
const char PARAM14[] PROGMEM = "Shift Odds: (lower is faster)";

const char *const PARAMS[] PROGMEM = { PARAM0, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7, PARAM8, PARAM9, PARAM10, PARAM11, PARAM12, PARAM13, PARAM14 };

enum {
  P_NUM,   //Numeric:  A -1 to 32767 integer value
  P_COL,   //Color:    A 1-3 digit octal number OR a character string.
  P_PER    //Percentage: A 0-100 percent chance
};

const int PARAM_TYPES[] PROGMEM = { P_NUM, P_NUM, P_NUM, P_NUM, P_NUM, P_NUM, P_COL, P_PER, P_PER };
  
const char STR0[] PROGMEM = "Unprotected Zepplayan Console";
const char STR1[] PROGMEM = "Defined colors: ";
const char STR2[] PROGMEM = "Unmatched command: ";
const char STR3[] PROGMEM = "Parameter must be an integer.";
const char STR4[] PROGMEM = "Percentage must be 0-100";
const char STR5[] PROGMEM = "Direction must be -1 to 1";
const char STR6[] PROGMEM = "Invalid Color ID";
const char STR7[] PROGMEM = "Parameter must be a positive integer";

const char *const STRS[] PROGMEM = { STR0, STR1, STR2, STR3, STR4, STR5, STR6, STR7 };

//  FUNCTIONS  ////////////////////////////////////////////////////////////////
void getMem(char* const* table, int index)
{
  //Copy string from a table into active buffer.
  strcpy_P(BUFFER, (char *)pgm_read_ptr(&(table[index])));
}

unsigned long getMemCol(int index)
{
  return pgm_read_dword(&COL_VALS[index]);
}

void printMsg(char* const* table, int index)
{
  getMem( table, index);
  delay(2);
  BlueSerial.print(BUFFER);
}

void printlnMsg(char* const* table, int index)
{
  getMem( table, index);
  delay(2);
  BlueSerial.println(BUFFER);
}
void showHelp()
{
  BlueSerial.println();
  printlnMsg(STRS, 0);
  printMsg(STRS, 1);
  /* Display the color names and keywords */
  for(int i=0;i<=MAX_COLS;++i)
  {
    BlueSerial.print(" ");
    printMsg(COLS, i);
  }
  BlueSerial.println("\n");

  for(int i=0;i<=MAX_CMDS;++i)
  {
    printMsg(CMDS, i);
    BlueSerial.print(":\t");
    printlnMsg(DESCS, i);
  }
}

int findInTable(char* const* table, int len)
{
  //Search in the specified table up to index len for a partial match.
  char c;
  int at=-1;
  
  for (int i=0;i<=len;++i)
  {
    getMem(table,i);

    int idx=0;
    while( idx < strlen(CMDBUF)  && idx < strlen(BUFFER) )
    {
      c=toupper( CMDBUF[idx] );
    
      if ( c != BUFFER[idx] )
      {
        idx = 0;
        break;
      }
      idx++;
    }
    
    if (idx > 0)
    {
      if (at!=-1)
      {
        at = -2;
        break;
      }
      at = i;
    }
  }
  return at;
}

void endParse( bool changed)
{
  //I love globals, don't you? 
  MODE_CHANGED = changed;

  if (MODE_CHANGED)
  {
    MODE_NEW = CMD;
  }

  CMD = -1;
  PARAM = 0;
}

bool parseInt(int &val, int int_type)
{
  //Try to convert CMDBUF to a number.  Returns false if not int.  value as val param, 
  char *c;
  char *endptr;

  c = CMDBUF;
  // Convert the string to a long integer
  val = (int)strtol(c, &endptr, 10);
  if (endptr != c+(strlen(CMDBUF)))
  {
    if (int_type != INT_COLOR)
    {
      //Failed to parse.
      printlnMsg(STRS, int_type);
    }
    return false;
  }

  bool bad=false;
  //Bounds checking
  switch(int_type)
  {
    case INT_PERCENT:
      if (val < 0 || val > 100)
      {
        bad = true;
      }
      break;
    case INT_DIRECTION:
      if (val < -1 || val > 1)
      {
        bad = true;
      }
      break;
    case INT_COLOR:
      if (val < 0 || val > 8)
      {
        bad = true;
      }
      break;
    case INT_POSITIVE:
      if (val < -1)
      {
        bad = true;
      }
      break;
    default:
      //INT_DECIMAL and INT_COLOR are silent.
      break;
  }

  if (bad && int_type != INT_COLOR)
  {
    printlnMsg(STRS, int_type);
  }
  return true;
}

bool parseColor(COLOR &color)
{
  //A color is either 1-3 0-8 values (eg: 3, 43, 008) or a named color like RED.
  //Color is returned in parameter.  true/false as rval for parsing success/fail

  int val;
  color.l = 0;
  if (parseInt(val, INT_COLOR))
  {
    //Might be a numeric  (hand parsing because it's easier)
    if (strlen(CMDBUF)<0 || strlen(CMDBUF)>3)
    {
      return false;
    }
    for (int i=0;i<strlen(CMDBUF) && i<3;++i)
    {
      if (CMDBUF[i] >= '0' && CMDBUF[i] <= '8')
      {
        color.c[2-i] = (unsigned char)clamp((int)(CMDBUF[i] - '0') * 32);
      }
      else
      {
        return false;
      }
    }
    return true;
  }
  else
  {
    //Non-numeric
    val = findInTable(COLS, MAX_COLS);
    if (val > -1)
    {
      color.l = getMemCol(val);
      return true;
    }
  }
  return false;
}

void makeBandWidth(int width)
{
  if ((unsigned int)(width*NUM_BANDS) > NUM_LEDS) //trim band width
  {
    BAND_WIDTH = floor(NUM_LEDS/NUM_BANDS);
  }
  else
  {
    BAND_WIDTH = width;
  }
}

void parseParams()
{
  int val;
  COLOR color;
          
  switch(CMD)
  {
    case CMD_FLASHLIGHT: // toggle flashlight
      FLASHLIGHT = !FLASHLIGHT;
      endParse(true);
      return;
    case CMD_HELP:  // display help
      showHelp();
      endParse(false);
      return;
    case CMD_QUIT:  //Reset everything to default.
      initializeParameters();
      endParse(true);
      return;
    case CMD_RND:
    case CMD_CHAOS:
      if (PARAM > 0)
      {       
        if ( parseInt(val, INT_DECIMAL) )
        {
          RAND_ON_MS = val;
          endParse(true);
          return;
        }
      }

      printMsg(PARAMS, 0);
      
      break;
    case CMD_COLOR:
      if (PARAM > 0)
      {
        if(parseColor( color ))
        {
          PIXELS[0].l = color.l;
          endParse(true);
          return;
        }
      }
      printMsg(PARAMS, 6);
      break;
    case CMD_WIDTH:
      if (PARAM > 0)
      {
        if(parseInt(val,INT_DECIMAL))
        {
          makeBandWidth(val);
          endParse(true);
          return;
        }
      }

      printMsg(PARAMS, 5); //pixels per band message

      break;
    case CMD_PRIDE:
      NUM_BANDS=6;
      makeBandWidth(-1);
      for (int i=0;i<NUM_BANDS;++i)
      {
        BANDS[i].l = getMemCol(i);
      }
      endParse(true);
      return;
    case CMD_LIST:
      if (PARAM > 0)
      {
        if(parseColor( color ))
        {
          //Valid color store and increase number of bands.
          BANDS[ NUM_BANDS ].l = color.l;
          NUM_BANDS++;
          if (NUM_BANDS == MAX_BANDS)
          {
            //Don't exceed memory.
            makeBandWidth(PTMP);
            endParse(true);
            return;
          }
          printMsg(PARAMS, 10);
        }
        else
        {
          if (CMDBUF[0] == 'x' || CMDBUF[0] == 'X')
          {
            makeBandWidth(-1);
            endParse(true);
            return;
          }
          else
          {
            printMsg(STRS, 6);
          }
        }
      }
      if (PARAM == 0)
      {
        NUM_BANDS = 0;
        makeBandWidth(1);
        printMsg(PARAMS, 10);
      }
      break;
    case CMD_MAXBRIGHT:
      if (PARAM > 0)
      {
        int val;
        if (parseInt(val,INT_PERCENT))
        {
          MAXBRIGHT = val;
          rain.mMaxBrightness=round( ((float)MAXBRIGHT/100.0f) * (float)255 );
          endParse(true);
          return;
        }
      }
      
      printlnMsg(STRS, 4);

      break;
    case CMD_RUN:
      if (PARAM == 3)
      {
        //crossfade ms
        if(parseInt(val, INT_DECIMAL))
        {
          RUN_XFADE_MS = val;
          RUN_DIR = (-1*PTMP);
          endParse(true);
          return;
        }
        PARAM=2;
        printMsg(PARAMS, 2);
        break;
      }
      if (PARAM == 2)
      {
        //hold ms
        if(parseInt(val,INT_DECIMAL))
        {
          RUN_ON_MS = val;
          printMsg(PARAMS, 2);
          break;
        }
        else
        {
          PARAM=1;
          printMsg(PARAMS,0);
        }
        break;
      }
      if (PARAM == 1 )
      {
        //Direction
        if(parseInt(val,INT_DIRECTION))
        {
          if (val==0)
          {
            RUN_DIR=0;
            endParse(true);
          }
          PTMP = val;
          printMsg(PARAMS, 0);
        }
        else
        {
          PARAM=0;
          printMsg( PARAMS, 9);
        }
        break;
      }
      if (PARAM == 0 )
      {
        printMsg(PARAMS, 9);
      }
      break;
                                   
    case CMD_BLINK:
      if (PARAM == 4)
      {
        if (parseInt(val,INT_DECIMAL))
        {
          BLINK[BLINK_IN] = val;
          BLINKING=true;
          endParse(true);
          return;
        }
        printMsg(PARAMS,4);
        break;
      }
      if (PARAM == 3)  //FadOut ms
      {
        if (parseInt(val,INT_DECIMAL))
        {
          BLINK[BLINK_OUT] = val;
          printMsg(PARAMS,4);
          break;
        }
        printMsg(PARAMS,3);
        return;
      }
      if (PARAM == 2) //OFF For
      {
        if (parseInt(val,INT_DECIMAL))
        {
          BLINK[BLINK_OFF] = val;
          printMsg(PARAMS,3);
          break;
        }
        printMsg(PARAMS, 1);
        return;
      }
      if (PARAM == 1) //ON For
      {

        if (CMDBUF[0] == 'x' || CMDBUF[0] == 'X')
        {
          BLINKING=false;
          endParse(true);
          return;
        }
        else if ( parseInt(val, INT_DECIMAL) )
        {
          BLINK[BLINK_ON] = val;
          printMsg(PARAMS, 1);
          break;
        }
        PARAM=0;
      }
      if (PARAM == 0)
      {
        printMsg(PARAMS, 13);
      }
      break;   
    case CMD_RAIN:
      if (PARAM > 1)
      {
        if (parseInt(val,INT_POSITIVE))
        {
          rain.mDecayRate = val;
          endParse(true);
          return;
        }
        printMsg(PARAMS, 0);
        break;
      }
      if (PARAM == 1)
      {
        if (parseInt(val,INT_POSITIVE))
        {
          rain.mShiftOdds = val;
          printMsg(PARAMS, 14);
          break;
        }
        PARAM = 0;
      }
      if (PARAM == 0)
      {
        printMsg(PARAMS, 12);
      }
      break;
    case CMD_SPARKLE:
      if (PARAM == 3)
      {
        if (parseInt(val, INT_DECIMAL)) //int
        {
          SPARKLE_MS = val;
          endParse(true);
          return;
        }
        else
        {
          PARAM = 2;
        }
        break;
      }
      if (PARAM == 2) //Sparkle color
      {
        if(parseColor( color ))
        {
          SPARKLE_COLOR.l = color.l;
          printMsg(PARAMS, 11); //Sparkle MS.
        }
        else
        {
          PARAM == 1;
        }
        break;
      }
      if (PARAM == 1) //Sparkle frequency
      {
        if (parseInt(val, INT_DECIMAL))
        {
          if (val >= 0 && val <= 100)
          {
            SPARKLE_CHANCE=val;
            if (SPARKLE_CHANCE==0)
            {
              //turn off sparkles early abort.
              endParse(true);
              return;
            }
            printMsg(PARAMS, 6);
            break;
          }
          printMsg(PARAMS, 8);
        }
        PARAM = 0;
        break;
      }
      if (PARAM == 0)
      {
        printMsg(PARAMS, 8);
      }
      break;
    default:    //Something wrong.  Reset.
      endParse(false);
      break;
  }
  
  PARAM++;
}

void parseInput()
{
  if (CMD < 0)
  {
    //This is a command, how exciting!
    CMD = findInTable(CMDS, MAX_CMDS);
    if (CMD < 0)
    {
      //Print error message.
      //Unable to uniquely match
      printMsg(STRS, 2);
      BlueSerial.println(CMDBUF);
    }
    else
    {
      //Echo the command.
      printlnMsg(CMDS,CMD);
    }
  }
  else
  {
    //Echo what was sent.
    BlueSerial.println(CMDBUF);
  }
  if (CMD >= 0)
  {
    parseParams();
  }
}

void handleInput()
{
  //Checks to see if user has entered something, parses if true.

  while (BlueSerial.available()) 
  {
    CMDBUF[CMDBUFIDX] = BlueSerial.read();

    if ((CMDBUF[CMDBUFIDX] == '\r'
        || CMDBUF[CMDBUFIDX] == '\n'
        || CMDBUF[CMDBUFIDX] == '\0'
        || CMDBUFIDX==30) && CMDBUFIDX > 0)
    {
      delay(20);
      CMDBUF[CMDBUFIDX] ='\0';
      parseInput();

      CMDBUFIDX = -1;
    }
    if (!(CMDBUFIDX==0
        && ( CMDBUF[CMDBUFIDX] == '\r' 
           ||CMDBUF[CMDBUFIDX] == '\n' )))
    {
      CMDBUFIDX++;
    }
  }
}

#endif BLUE_UI_H
