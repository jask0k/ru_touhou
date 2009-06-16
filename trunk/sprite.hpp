#ifndef _SPRITE_HPP
#define _SPRITE_HPP

#include "SDL_opengl.h"
#include "SDL.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "copypasta.hpp"
typedef std::vector<int> vint;
typedef std::vector<std::vector<int> > vvint;

class CSpriteSheet{
private:
  GLuint texture_handle;
  //хендлер текстуры
  SDL_Surface *sdl_texture;
  //поверхность для SDL
  SDL_Rect rectangle;
  //rectangle.w и h -> размер спрайта
  //rectangle.x и у -> количество спрайтов по вертикали и горизонтали
  vvint* animations;
  //вектор векторов анимаций
  
public:
  CSpriteSheet(char* filename);
  std::string sheetname;
  vvint* parse_props(char* filename);
  void draw(GLuint animation, GLuint state, GLfloat x, GLfloat y, GLfloat rotation=0.0f);
  void draw(GLuint frame, GLfloat x, GLfloat y, GLfloat rotation=0.0f); 
  //рисование с плавающими координатами
  void draw_int(GLuint frame, GLint x, GLint y);
  //рисование с целыми координатами
  GLuint get_width(){return rectangle.w;}
  GLuint get_height(){return rectangle.h;}
  GLuint get_count(){return rectangle.x*rectangle.y;}
};

class CSpriteSheetManager{
public:
  CSpriteSheet* load(char* filename);
  CSpriteSheet* dispatch(std::string sheetname);
  std::map<std::string,CSpriteSheet*> collection;
};

class CSprite{
public:
  CSprite(std::string sheetname, CSpriteSheetManager* manager, GLuint frame_no);
  void set_position(GLfloat x, GLfloat y, GLfloat rotation=0.0f);
  void draw();
  void think();
private:
  GLfloat x,y;
  GLfloat rotation;
  CSpriteSheet* ssheet;
  GLuint frame;
  //  GLuint animation,state;
};

class CAnimatedSprite:public CSprite{
public:
  void draw();
private:
  GLuint animation,state;
};

#endif
