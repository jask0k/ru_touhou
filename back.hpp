#ifndef _BACK_HPP
#define _BACK_HPP

#include "SDL_opengl.h"
#include "SDL.h"
#include "copypasta.hpp"
#include "config.hpp"

class CBack{
private:
  GLuint flightY;
  GLuint ground_texture;
  GLint vy;
public:
  void init(const char* filename, GLint vy=4);
  void draw();
  void think();
  void set_speed(GLint vy);
};

#endif
