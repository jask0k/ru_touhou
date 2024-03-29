#ifndef _BACK_HPP
#define _BACK_HPP

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include "copypasta.hpp"
#include "config.hpp"

class CBack{ //tolua_export
private:
  GLfloat flightY;//<-координаты угла текстуры, с которого начинается прорисовка
  GLfloat flightX;//<|
  GLuint ground_texture;//текстура поверхности земли
  GLfloat vx,vy;//скорости движения текстуры
  GLfloat rx,ry,rz;//углы наклона земли
  GLfloat vrx,vry,vrz;//скорость вращения поверхности земли
  GLfloat fog_density;//плотность тумана
  GLfloat fog_colour[4];//цвет тумана
  GLfloat vfog;//скорость изменения плотности тумана
  
public:
  void init(const char* filename, GLfloat vy=0.01f); //tolua_export
  void draw();
  void think();
  //tolua_begin
  void set_speed(GLfloat vy, GLfloat vx=0.0f);
  void set_rotation(GLfloat rx=0.0f, GLfloat ry=0.0f, GLfloat rz=0.0f);
  void set_rotation_speed(GLfloat rvx=0.0f, GLfloat rvy=0.0f, GLfloat rvz=0.0f);
  void set_fog_colour(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha=1.0f);
  void set_fog_density(GLfloat density, GLfloat vfog=0.0f);
};
//tolua_end
namespace game{
  extern CBack* background; //tolua_export
}

#endif
