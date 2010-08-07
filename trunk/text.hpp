#ifndef _TEXT_HPP
#define _TEXT_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

class CSpriteSheet;

#include "config.hpp"
#include "sprite.hpp"

enum text_align{
  ALIGN_RIGHT = -1,
  ALIGN_CENTER = 0,
  ALIGN_LEFT = 1
};

enum text_layer{
  LAYER_GAME,//слой на игровом поле
  LAYER_PANEL//слой на панельке
};


class CLabel{
public:
  CLabel(GLint x, GLint y, std::string text, CSpriteSheet* font, text_layer layer, GLuint decay=0);
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
  text_layer layer;
  friend class CLabelManager;
};

class CLabelManager{
public:
  CLabelManager();
  ~CLabelManager();
  void draw(text_layer layer);
  void think();
  int font_load(std::string fontname);
  GLuint text_add(GLint x, GLint y, std::string text, GLuint font_n, text_layer layer, GLuint decay=0);
  CLabel* get_label(GLuint handle); 
  void destroy_label(GLuint handle);
  
private:

  GLuint min_handle;
  std::map<GLuint,CLabel*> labels;
  std::vector<CSpriteSheet*> fonts;
};
 
  
namespace game{
  extern CLabelManager* lmanager;
}
#endif
