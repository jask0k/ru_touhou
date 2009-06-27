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
#include "config.hpp"

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
  void draw(GLuint animation, GLuint state, GLfloat x, GLfloat y, GLfloat rotation=0.0f, GLfloat scale=1.0f);
  void draw(GLint frame, GLfloat x, GLfloat y, GLfloat rotation=0.0f, GLfloat scale=1.0f); 
  //рисование с плавающими координатами
  void draw_int(GLuint frame, GLint x, GLint y);
  //рисование с целыми координатами
  GLuint get_width(){return rectangle.w;}//ширина спрайта
  GLuint get_height(){return rectangle.h;}//высота спрайта
  GLuint get_count(){return rectangle.x*rectangle.y;}//количество спрайтов
  GLuint get_frames(GLuint animation);
  GLuint get_pause(GLuint animation, GLuint state);
};

class CSpriteSheetManager{
private:
  std::map<std::string,CSpriteSheet*> collection;
public:
  CSpriteSheet* load(char* filename);
  CSpriteSheet* dispatch(std::string sheetname);
};

class CSprite{
public:
  CSprite(CSpriteSheet* ssheet, GLint frame_no);
  CSprite(CSpriteSheet* ssheet, GLuint anim_no);
  void set_position(GLfloat x, GLfloat y, GLfloat rotation=0.0f);//установка положения
  void set_speed(GLfloat v_x, GLfloat v_y, GLfloat v_r=0.f);//установка скорости
  void set_angle(GLfloat v, GLfloat angle);//установка вектора скорости по углу
  void set_tint(GLfloat red, GLfloat green, GLfloat blue);//установка окраски
  void clear_tint();
  void set_alpha(GLfloat amount);//установка альфоты
  void set_alpha_speed(GLfloat amount);//установка скорости изменения альфоты
  void set_scale(GLfloat scale);//установка увеличения/уменьшения спрайта
  void set_decay(GLuint decay){decay_active = true; decay_timer = decay;};
  void draw();//отрисовка
  decay_state think();//анимация, движение, и т.д.
  
private:
  GLfloat x,y;//координаты центра спрайта
  GLfloat rotation;//поворот относительно центра
  GLfloat alpha;//коэффициент альфа-канала спрайта
  GLfloat tint_r,tint_g,tint_b;//окрашивание спрайта
  CSpriteSheet* ssheet;//указатель на спрайтовый лист

  GLfloat v_alpha;//скорость изменения альфаканала
  GLfloat v_x,v_y,v_r;//скорость движения и вращения спрайта

  GLint frame;//номер кадра
  GLuint animation,state;//номер анимации и фрейма
  GLuint animation_timer;//таймер переключения анимации
  
  GLuint decay_timer;//таймер исчезновения

  GLboolean animation_active;//флаг активности анимации
  GLboolean decay_active;//флаг активности таймера полного исчезновения
  GLfloat scale;

};

class CSpriteManager{
public:
  CSpriteManager(CSpriteSheetManager* ssmanager);
  void think();
  void draw();
  GLuint create_sprite(std::string spritesheet, GLint frame_no);
  GLuint create_sprite(std::string spritesheet, GLuint animation);
  
  GLuint destroy_sprite(GLuint handle);
  CSprite* get_sprite(GLuint handle);
private:
  CSpriteSheetManager* ssmanager;
  std::map<GLuint,CSprite*> collection;
  GLuint free_handle;//минимальный свободный псевдоуказатель
};

#endif
