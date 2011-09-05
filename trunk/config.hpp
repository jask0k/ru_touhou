#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#define DEFAULT_X 640
#define DEFAULT_Y 480
#define DEFAULT_COLOUR 32

#define GAME_FULLSCREEN 1
#define GAME_WINDOWED 0
#define WINDOW_DEFAULT GAME_WINDOWED

#define GAME_FIELD_WIDTH 384
#define GAME_FIELD_HEIGHT 448

#define JOY_DEAD_ZONE 300

#define CURRENT_RES RES_640x480

#ifdef WIN32

#define CONFIG_FILE "./config.dat"

#else

//здесь будет конфиг в $HOME
#define CONFIG_FILE "./config.dat"

#endif

#define GAME_DATAFILE "th_ru.dat"

enum decay_state{
  STILL_ALIVE,
  DECOMPOSED
};

#endif
