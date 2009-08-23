#ifndef _HERO_HPP
#define _HERO_HPP

#include "sprite.hpp"
#include "SDL_opengl.h"
#include "SDL.h"
#include <cmath>
#include "config.hpp"


class CHero{
public:
  GLfloat vx,vy;//скорость
  GLfloat x,y;//координаты героя
  GLfloat max_speed;
  GLuint sprite_no;//спрайт персонажа
  CHero(const char*);
  CHero(std::string);
  ~CHero();
  void think();//движение
  void draw();
  void set_position(GLfloat x0, GLfloat y0);
  void set_speed(GLfloat vx0, GLfloat vy0);
  void set_speed_angle(GLfloat velocity, GLfloat direction);
  GLfloat get_height();
};

namespace game{
  extern CHero* hero;
}
#endif
