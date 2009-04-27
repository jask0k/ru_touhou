#ifndef _SPRITE_HPP
#define _SPRITE_HPP

#include "SDL_opengl.h"
#include "SDL.h"

#include <vector>
using std::vector;

class CSpriteSheet{
private:
  GLuint texture_handle;
  SDL_Surface *sdl_texture;
  
public:
  CSpriteSheet(char* filename);
  char* sheetname;
  vector<vector<int> >* animations;
  vector<vector<int> >* parse_animations(char* anim_defs);
  void draw(GLuint animation, GLfloat state, GLfloat x, GLfloat y);
  void draw(GLuint animation, GLfloat state, GLfloat x, GLfloat y, GLfloat rotation);
};

class CSpriteSheetManager{
public:
  CSpriteSheet* load(char* filename);
  CSpriteSheet* dispatch(char* sheetname);
  vector<CSpriteSheet*> collection;
};

class CSprite{
public:
  void setposition();
  void draw();
  CSprite(char* sheetname, CSpriteSheetManager* manager);
private:
  GLfloat x,y;
  GLfloat rotation;
  CSpriteSheet* ssheet;
  GLuint anmation,state;
};
#endif
