#ifndef _GAME_HPP
#define _GAME_HPP

#include "config.hpp"
#include "hero.hpp"
#include "sprite.hpp"
#include "copypasta.hpp"
#include "controller.hpp"

#include "SDL.h"
#include "SDL_opengl.h"
//состояния движка

enum EngineMainState{
  ENGINE_STATE_MAIN_MENU,
  ENGINE_STATE_GAME,
  ENGINE_STATE_QUIT,
  ENGINE_STATE_PAUSED
};

struct EngineState{
  EngineMainState main_state;
  bool active;
};
  
class CFrameManager{
private:
  GLuint begin_time;
  GLfloat FPS;
  GLfloat averageFPS;
  unsigned long frames;
public:
  CFrameManager();
  GLfloat get_FPS();
  GLfloat get_aFPS();
  void begin_frame();//начало кадра -- сохраняем время начала кадра
  void end_frame();//конец кадра -- ждём время, обновляем FPS
};

class CEngine{
private:
  void think();//то, что будет выполнятся каждый кадр(кроме рисования), математика и всё такое
  SDL_Surface* screen;//поверхность экрана
  GLuint ui_background;
  int read_config();//чтение конфига
  int write_config();//запись конфига
  void handle_events();
  EngineState state; //состояние движка
  unsigned long frames; //количество кадров от начала игры
  CHero* hero;//ГГ
  CSpriteSheetManager* ssmanager;
  CFrameManager* fps_manager;
  CController* controller;
public:
  int xres,yres;//разрешение экрана
  int colour;//цветность в битах
  int fullscreen;
  CEngine();//конструктор, в нём мы инициализируем опенгл
  ~CEngine();
  void new_game();//начало игры
  void loop();//основной цикл игры
  void draw();//рендеринг картинки, менюшек
  void draw_game();//рисование гуя и игры(нужно для красивого заблюривания во время паузы)
  
};
#endif
