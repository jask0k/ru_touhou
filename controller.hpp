#ifndef _CONTROLLER_HPP
#define _CONTROLLER_HPP
#include "SDL.h"
#include "SDL_opengl.h"
#include <set>
#include <cmath>
#include <iostream>
enum EButton{
  B_NIL = 0,
  B_UP = 1,
  B_DOWN = 2,
  B_LEFT = 3,
  B_RIGHT = 4,
  B_ATTACK,
  B_BOMB,
  B_FOCUS,
  B_PAUSE,
  B_SKIP
};
#define STATE_DOWN true
#define STATE_UP false

struct controller_state{
  GLfloat direction;//угол наклона вектора действия
  GLfloat strength;//0..1 процент действия
  bool focus;
  bool attack;
  bool skip;
};
class CController{
public:
  CController();
  ~CController();
  int handle_event(SDL_Event* event);
  void button(EButton key, bool key_state);
  int button(SDLKey key, bool key_state);
  void move_analog(GLfloat x, GLfloat y);
  GLfloat get_kx();
  GLfloat get_ky();
  struct controller_state get_state();
  int set_state(struct controller_state);
  
  EButton get_confirm();//смотрит, нажата ли кнопка атаки или бомбы и 
  //очищает их состояние, нужно для меню
  int get_pause();
  
private:
  void analog_sync();
  std::set<EButton>* state;
  struct{
    GLfloat dir;
    GLfloat pow;
  } analog_state;
};
#endif
