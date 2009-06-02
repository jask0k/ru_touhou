#ifndef _BACK_HPP
#define _BACK_HPP

#include "SDL_opengl.h"
#include "SDL.h"
#include "copypasta.hpp"

class CBack{
private:
  GLuint flightY;
  GLuint ground_texture;
  GLint vy;
public:
  void init(std::string filename, GLint vy=4);
  void draw();
  void think();
};

#endif
