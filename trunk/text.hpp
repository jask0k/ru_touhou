#ifndef _TEXT_HPP
#define _TEXT_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "config.hpp"
#include "sprite.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

enum text_align{
  ALIGN_RIGHT = -1,
  ALIGN_CENTER = 0,
  ALIGN_LEFT = 1
};



class CLabel{
public:
  CLabel(GLint x, GLint y, std::string text, CSpriteSheet* font, GLuint decay=0);
  ~CLabel();
  void draw();
  decay_state think();
  void change_text(std::string text);
  void change_text(GLfloat number);
private:
  GLuint decay_timer;
  bool decay_active;
  GLint x,y;
  text_align align;
  std::string text;
  CSpriteSheet* font;
};

class CText{
public:
  ~CText();
  void draw();
  void think();
  int font_load(std::string fontname);
  CLabel* text_add(GLint x, GLint y, std::string text, GLuint font_n, GLuint decay=0);
private:
  std::vector<CLabel*> labels;
  std::vector<CSpriteSheet*> fonts;
};
 
  
#endif
