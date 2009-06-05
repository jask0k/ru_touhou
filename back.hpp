#ifndef _BACK_HPP
#define _BACK_HPP

#include "SDL_opengl.h"
#include "SDL.h"
#include "copypasta.hpp"
#include "config.hpp"

class CBack{
private:
  GLfloat flightY;
  GLuint ground_texture;
  GLfloat vy;
public:
  void init(const char* filename, GLfloat vy=0.05f);
  void draw();
  void think();
  void set_speed(GLfloat vy);
};

#endif
