#ifndef _HERO_HPP
#define _HERO_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <iostream>
#include <cmath>

#include "sprite.hpp"
#include "config.hpp"
#include "script.hpp"
//tolua_begin
class CHero{
public:
  GLfloat vx,vy;//скорость
  GLfloat x,y;//координаты героя
  GLfloat max_speed;
  //  GLuint sprite_no;//спрайт персонажа
  CSprite* sprite;
  CHero(const char* scriptname);
  ~CHero();
  //tolua_end
  void think();//движение
  void draw();
  //tolua_begin
  void set_position(GLfloat x0, GLfloat y0);
  void set_speed(GLfloat vx0, GLfloat vy0);
  void set_speed_angle(GLfloat velocity, GLfloat direction);
  GLfloat get_height();
};
//tolua_end

namespace game{
  extern CHero* hero; //tolua_export
}
#endif

