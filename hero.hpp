#ifndef _HERO_HPP
#define _HERO_HPP

#include "sprite.hpp"
#include "SDL_opengl.h"
#include "SDL.h"
class CHero{
public:
  GLuint texture;
  GLfloat speed,angle;
  GLfloat vx,vy;
  unsigned int w,h;
  CSprite* sprite;
  CHero(char*, CSpriteSheetManager*);
  void think();
  void draw();
};
#endif
