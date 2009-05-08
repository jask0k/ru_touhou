#ifndef _HERO_HPP
#define _HERO_HPP

#include "sprite.hpp"
#include "SDL_opengl.h"
#include "SDL.h"
class CHero{
public:
  GLfloat x,y;
  GLuint texture;
  GLfloat speed,angle;
  GLfloat vx,vy;
  unsigned int w,h;
  CSpriteSheet* spritesheet;
  CHero();
  CHero(char*);
  GLuint load_texture(SDL_Surface*);
  void think();
  void draw();
};
#endif
