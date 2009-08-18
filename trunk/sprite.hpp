#ifndef _SPRITE_HPP
#define _SPRITE_HPP

#include "SDL_opengl.h"
#include "SDL.h"
#include "SDL_rwops.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "copypasta.hpp"
#include "config.hpp"
#include "SDL_rwops_zzip.h"

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
  GLuint get_animations(){return animations -> size();};
  GLuint get_frames(GLuint animation);
  GLuint get_pause(GLuint animation, GLuint state);
};

enum Layer{
  LAYER_BACKGROUND=0,
  LAYER_ENEMY_BULLET,
  LAYER_ENEMY,
  LAYER_HERO_BULLET,
  LAYER_HERO,
  LAYER_EMBLEM
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
  CSprite(CSpriteSheet* ssheet, Layer layer=LAYER_EMBLEM);
  void set_position(GLfloat x, GLfloat y, GLfloat rotation=0.0f);//установка положения
  void set_speed(GLfloat v_x, GLfloat v_y, GLfloat v_r=0.f);//установка скорости
  void set_angle(GLfloat v, GLfloat angle);//установка вектора скорости по углу
  void set_tint(GLfloat red, GLfloat green, GLfloat blue);//установка окраски
  void clear_tint();
  void set_alpha(GLfloat amount);//установка альфоты
  void set_alpha_speed(GLfloat amount);//установка скорости изменения альфоты
  void set_scale(GLfloat scale, GLfloat v_scale=0.0f);//установка увеличения/уменьшения спрайта
  void set_scale_speed(GLfloat v_scale){this->v_scale=v_scale;};
  void set_decay(GLuint decay){decay_active = true; decay_timer = decay;};
  void set_layer(Layer layer){this->layer=layer;};
  void set_blur(GLboolean blur=true){this->blur=blur;};
  void set_alpha_limit(GLfloat min_alpha, GLfloat max_alpha){this->min_alpha = min_alpha;this -> max_alpha = max_alpha;};
  void set_scale_limit(GLfloat min_scale, GLfloat max_scale){this->min_scale = min_scale;this -> max_scale = max_scale;};
  int start_animation(GLint animation, GLint next_animation=-1);
  int set_frame(GLint frame){this->frame=frame;animation_active = false; return 0;};
  void set_follow(GLuint follow){this -> follow = follow;};
  GLfloat get_width(){return (GLfloat)(ssheet->get_width())*scale;};
  GLfloat get_height(){return (GLfloat)(ssheet->get_height())*scale;};
  
  void draw();//отрисовка
  decay_state think();//анимация, движение, и т.д.
  
  
private:
  CSpriteSheet* ssheet;//указатель на спрайтовый лист
  GLfloat x,y;//координаты центра спрайта
  GLfloat rotation;//поворот относительно центра
  GLfloat alpha;//коэффициент альфа-канала спрайта
  GLfloat tint_r,tint_g,tint_b;//окрашивание спрайта

  GLfloat v_alpha;//скорость изменения альфаканала
  GLfloat min_alpha,max_alpha;
  GLfloat v_x,v_y,v_r;//скорость движения и вращения спрайта
  GLfloat v_scale;//скорость изменения размера спрайта
  GLfloat min_scale,max_scale;

  GLint frame;//номер кадра

  GLint animation,state;//номер анимации и фрейма
  GLint animation_timer;//таймер переключения анимации
  GLint next_animation;//следующая анимация
  //TODO: надо бы всё это забабахать в структуру
  
  GLuint decay_timer;//таймер исчезновения

  GLboolean animation_active;//флаг активности анимации
  GLboolean decay_active;//флаг активности таймера полного исчезновения
  GLfloat scale;
  
  GLboolean blur;

  Layer layer;

  GLuint follow;
  friend class CSpriteManager;
};

class CSpriteManager{
public:
  CSpriteManager();
  void think();
  void draw(Layer layer);
  GLuint create_sprite(std::string spritesheet, Layer layer=LAYER_EMBLEM);
  
  GLuint destroy_sprite(GLuint handle);
  CSprite* get_sprite(GLuint handle);
private:
  std::map<GLuint,CSprite*> collection;
  GLuint free_handle;//минимальный свободный псевдоуказатель
};

namespace game{
  extern CSpriteManager* smanager;
  extern CSpriteSheetManager* ssmanager;
}

#endif
