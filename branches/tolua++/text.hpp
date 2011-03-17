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
//tolua_begin
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
  //tolua_end
  void draw();
  decay_state think();
  //tolua_begin
  void change_text(std::string text);
  void change_text(GLfloat number);
  //tolua_end
private:
  GLuint decay_timer;
  bool decay_active;
  GLint x,y;
  text_align align;
  std::string text;
  CSpriteSheet* font;
  text_layer layer;
  friend class CLabelManager;
  //tolua_begin
};

class CLabelManager{
public:
  CLabelManager();
  ~CLabelManager();
  //tolua_end
  void draw(text_layer layer);
  void think();
  //tolua_begin
  int font_load(CSpriteSheet* fontsheet);
  GLuint text_add(GLint x, GLint y, std::string text, GLuint font_n, text_layer layer, GLuint decay=0);
  CLabel* get_label(GLuint handle); 
  void destroy_label(GLuint handle);
  //tolua_end;
private:

  GLuint min_handle;
  std::map<GLuint,CLabel*> labels;
  std::vector<CSpriteSheet*> fonts;
}; //tolua_export
 
  
namespace game{
  extern CLabelManager* lmanager; //tolua_export
}
#endif
