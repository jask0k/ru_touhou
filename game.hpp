#ifndef _GAME_HPP
#define _GAME_HPP

#include "config.hpp"
#include "hero.hpp"

#include "SDL.h"
#include "SDL_opengl.h"
//состояния движка
enum EngineState{
  ENGINE_STATE_MENU,
  ENGINE_STATE_GAME,
  ENGINE_STATE_QUIT,
  ENGINE_STATE_PAUSED
};
  
class CEngine{
private:
  void think();//то, что будет выполнятся каждый кадр(кроме рисования), математика и всё такое
  SDL_Surface* screen;//поверхность экрана
  int read_config();//чтение конфига
  int write_config();//запись конфига
  EngineState state; //состояние движка
  unsigned long frames; //количество кадров от начала игры
  CHero* hero;//ГГ
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
