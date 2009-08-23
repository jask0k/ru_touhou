#include "hero.hpp"
#include <iostream>
#include "SDL_image.h"

CHero::CHero(const char* texture_file):
  vx(0),vy(0),x(GAME_FIELD_WIDTH/2),y(100),max_speed(8),
  sprite_no(game::smanager->create_sprite(texture_file, LAYER_HERO)){
  (game::smanager->get_sprite(sprite_no))->set_position(x,y);
  (game::smanager->get_sprite(sprite_no))->set_scale(0.0625f);
}

CHero::CHero(std::string texture_file):
  vx(0),vy(0),x(GAME_FIELD_WIDTH/2),y(100),max_speed(8),
  sprite_no(game::smanager->create_sprite(texture_file, LAYER_HERO)){
  (game::smanager->get_sprite(sprite_no))->set_position(x,y);
  (game::smanager->get_sprite(sprite_no))->set_scale(0.0625f);
}

CHero::~CHero(){
  //  delete sprite;
}

void CHero::draw(){
  (game::smanager->get_sprite(sprite_no)) -> draw();
}

void CHero::set_position(GLfloat x0, GLfloat y0){
  x = x0;
  y = y0;
  (game::smanager->get_sprite(sprite_no)) -> set_position(x0, y0);
}

void CHero::set_speed(GLfloat vx0, GLfloat vy0){
  vx = vx0 * max_speed;
  vy = vy0 * max_speed;
}

void CHero::set_speed_angle(GLfloat velocity, GLfloat direction){
  vx = cos(direction)*velocity*max_speed;
  vy = sin(direction)*velocity*max_speed;
}

GLfloat CHero::get_height(){
  CSprite* sprite = game::smanager -> get_sprite(sprite_no);
  return sprite -> get_width();
}

void CHero::think(){
  if (x+vx < 0)
    x = 0;
  else if (x+vx > GAME_FIELD_WIDTH)
    x = GAME_FIELD_WIDTH;
  else
    x += vx;
  if (y+vy < 0)
    y = 0;
  else if (y+vy > GAME_FIELD_HEIGHT)
    y = GAME_FIELD_HEIGHT;
  else
    y += vy;
  set_position(x,y);
}
