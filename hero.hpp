#ifndef _HERO_HPP
#define _HERO_HPP

#include "sprite.hpp"
#include "SDL_opengl.h"
#include "SDL.h"
class CHero{
public:
  GLuint texture;
  GLfloat speed,angle;
  GLfloat x,y;
  GLfloat vx,vy;
  unsigned int w,h;
  CSprite* sprite;
  CHero(char*, CSpriteSheetManager*);
  void think();
  void draw();
  void set_position(GLfloat x0, GLfloat y0);
  void set_speed(GLfloat vx0, GLfloat vy0);
  void set_speed_angle(GLfloat velocity, GLfloat direction);
};
#endif
