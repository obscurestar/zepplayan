#ifndef ENUMS_H
#define ENUMS_H

//This enum must be in same order as CMDS in blue_ui.
enum {
  CMD_RAIN,
  CMD_RND,
  CMD_CHAOS,
  CMD_PRIDE,
  CMD_COLOR,
  CMD_LIST,
  CMD_RUN,
  CMD_BLINK,
  CMD_MAXBRIGHT,
  CMD_FLASHLIGHT,
  CMD_SPARKLE,
  CMD_WIDTH,
  CMD_HELP,
  CMD_QUIT
};

enum {
  RUN_ON,
  RUN_FADE
};

enum {
  BLINK_ON,
  BLINK_OUT,
  BLINK_OFF,
  BLINK_IN
};

enum {
  TID_RAND,
  TID_RUN,
  TID_BLINK,
  TID_SPARKLE
};

enum {
  INT_SILENT = -1,    //Explicitly suppress message.
  INT_DECIMAL=3,
  INT_PERCENT=4,
  INT_DIRECTION=5,
  INT_COLOR=6,
  INT_POSITIVE=7
};

#endif //ENUMS_H
