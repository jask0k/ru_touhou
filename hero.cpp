#include "hero.hpp"
#include <iostream>
#include "SDL_image.h"

CHero::CHero(char* texture_file, CSpriteSheetManager* manager){
  vx = vy = speed = angle = 0;
  w = 32;
  h = 64;
  sprite = new CSprite(texture_file, manager, 0);
  sprite->set_position(100,100);
}

CHero::~CHero(){
  delete sprite;
}

void CHero::draw(){
  sprite -> draw();
}

void CHero::set_position(GLfloat x0, GLfloat y0){
  x = x0;
  y = y0;
  sprite -> set_position(x0, y0);
}
