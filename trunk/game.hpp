#ifndef _GAME_HPP
#define _GAME_HPP

class CScript;

#include "config.hpp"
#include "hero.hpp"
#include "sprite.hpp"
#include "copypasta.hpp"
#include "controller.hpp"
#include "text.hpp"
#include "script.hpp"
#include "enemy_bullet.hpp"
#include "particles.hpp"

#include "SDL.h"
#include "SDL_opengl.h"
#include <ctime>
#ifndef WIN32
#include <unistd.h>
#endif

namespace game{
  extern CEngine* engine;
}

//состояния движка
enum EngineMainState{
  ENGINE_STATE_MAIN_MENU,
  ENGINE_STATE_GAME,
  ENGINE_STATE_QUIT
  //  ENGINE_STATE_PAUSED
};

struct EngineState{
  EngineMainState main_state;
  bool active;
  bool screenshot;
  bool pause;
  bool lockdown;
  GLuint god_timer;
};
  
class CFrameManager{
private:
  GLfloat FPS;
  unsigned long frames;//количество кадров от начала игры
  CLabel* fps_label;
  GLfloat fps_ticks;
  GLuint last_ticks;
public:
  CFrameManager(CLabel* label);
  GLfloat get_FPS();//извлечение FPS
  // GLfloat get_aFPS();//извлечение среднего показателя FPS
  //  void begin_frame();//начало кадра -- сохраняем время начала кадра
  void wait();//конец кадра -- ждём время, обновляем FPS
};


class CEngine{
private:
  void think();//то, что будет выполнятся каждый кадр(кроме рисования), математика и всё такое
  SDL_Surface* screen;//поверхность экрана
  GLuint ui_background;//текстура панели
  int read_config();//чтение конфига
  int write_config();//запись конфига
  void handle_events();//обработка событий
  unsigned long int frames; //количество кадров от начала игры
  //  CHero* hero;//ГГ
  CFrameManager* fps_manager;
  CController* controller;
  CText* text;
public:
  int xres,yres;//разрешение экрана
  int colour;//цветность в битах
  int fullscreen;
  EngineState state; //состояние движка
  CEngine();//конструктор, в нём мы инициализируем опенгл
  ~CEngine();
  void new_game();//начало игры
  void loop();//основной цикл игры
  void draw();//рендеринг картинки, менюшек
  void draw_game();//рисование гуя и игры(нужно для красивого заблюривания во время паузы)
  int save_screenshot();
  unsigned long int get_frame(){return frames;};
  
};
#endif
