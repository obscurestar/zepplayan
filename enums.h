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
  CMD_HELP
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

#endif //ENUMS_H
