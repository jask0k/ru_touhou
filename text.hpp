#ifndef _TEXT_HPP
#define _TEXT_HPP

#include "SDL.h"
#include "SDL_opengl.h"
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

enum decay_state{
  STILL_ALIVE,
  DECOMPOSED
};

class CLabel{
public:
  CLabel(GLfloat x, GLfloat y, std::string text, CSpriteSheet* font, GLuint decay=0);
  ~CLabel();
  void draw();
  decay_state think();
  void change_text(std::string text);
  void change_text(GLfloat number);
private:
  GLuint decay_timer;
  bool decay_active;
  GLfloat x,y;
  text_align align;
  std::string text;
  CSpriteSheet* font;
};

class CText{
public:
  CText(CSpriteSheetManager* manager);
  ~CText();
  void draw();
  void think();
  int font_load(std::string fontname);
  CLabel* text_add(GLfloat x, GLfloat y, std::string text, GLuint font_n);
private:
  std::vector<CLabel*> labels;
  std::vector<CSpriteSheet*> fonts;
  CSpriteSheetManager* ssmanager;
};
 
  
#endif
