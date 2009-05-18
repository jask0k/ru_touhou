#include "hero.hpp"
#include <iostream>
#include "SDL_image.h"

CHero::CHero(char* texture_file, CSpriteSheetManager* manager){
  vx = vy = 0;
  x = y = 100;
  max_speed = 4;
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

void CHero::set_speed(GLfloat vx0, GLfloat vy0){
  vx = vx0 * max_speed;
  vy = vy0 * max_speed;
}

void CHero::set_speed_angle(GLfloat velocity, GLfloat direction){
  vx = cos(direction)*velocity*max_speed;
  vy = sin(direction)*velocity*max_speed;
}

void CHero::think(){
  if (x+vx < 0)
    x = 0;
  else if (x+vx > GAME_FIELD_WIDTH/2)
    x = GAME_FIELD_WIDTH/2;
  else
    x += vx;
  if (y+vy < 0)
    y = 0;
  else if (y+vy > GAME_FIELD_HEIGHT/2)
    y = GAME_FIELD_HEIGHT/2;
  else
    y += vy;
  sprite -> set_position(floor(x),floor(y));
}
