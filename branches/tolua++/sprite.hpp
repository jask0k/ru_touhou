#ifndef _SPRITE_HPP
#define _SPRITE_HPP

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rwops.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

#include "copypasta.hpp"
#include "config.hpp"
#include "physfsrwops.h"
#include "collection.hpp"

typedef std::vector<int> vint;
typedef std::vector<std::vector<int> > vvint;

class CSpriteSheet{//tolua_export
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
  //tolua_begin
  CSpriteSheet(const char* filename);
  ~CSpriteSheet();
  std::string sheetname;
  //tolua_end
  vvint* parse_props(const char* filename);
  void draw(GLuint animation, GLuint state, GLfloat x, GLfloat y, GLfloat rotation=0.0f, GLfloat scale=1.0f);
  void draw(GLint frame, GLfloat x, GLfloat y, GLfloat rotation=0.0f, GLfloat scale=1.0f); 
  //рисование с плавающими координатами
  void draw_int(GLuint frame, GLint x, GLint y);
  //рисование с целыми координатами
  //tolua_begin
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
  //tolua_end
private:
  std::set<CSpriteSheet*> collection;
public:
  //  CSpriteSheet* load(const char* filename);
  void add(CSpriteSheet* new_member);
  void del(CSpriteSheet* old_member);
  //  CSpriteSheet* dispatch(std::string sheetname);
  //tolua_begin
};

class CSprite{
public:
  CSprite(CSpriteSheet* ssheet, Layer layer=LAYER_EMBLEM);
  // CSprite(const char* ssheet, Layer layer=LAYER_EMBLEM);
  // CSprite(std::string ssheet, Layer layer=LAYER_EMBLEM);
  ~CSprite();
  void set_position(GLfloat x, GLfloat y, GLfloat rotation=0.0f);//установка положения
  //  void set_speed(GLfloat v_x, GLfloat v_y, GLfloat v_r=0.f);//установка скорости
  void set_angle(GLfloat v, GLfloat angle);//установка вектора скорости по углу
  void set_tint(GLfloat red, GLfloat green, GLfloat blue);//установка окраски
  void clear_tint();
  // void set_alpha(GLfloat amount);//установка альфоты
  // void set_alpha_speed(GLfloat amount);//установка скорости изменения альфоты
  // void set_scale(GLfloat scale, GLfloat v_scale=0.0f);//установка увеличения/уменьшения спрайта
  // void set_scale_speed(GLfloat v_scale){this->v_scale=v_scale;};
  // void set_decay(GLuint decay){decay_active = true; decay_timer = decay;};
  // void set_layer(Layer layer){this->layer=layer;};
  // void set_blur(GLboolean blur=true){this->blur=blur;};
  // void set_alpha_limit(GLfloat min_alpha, GLfloat max_alpha){this->min_alpha = min_alpha;this -> max_alpha = max_alpha;};
  // void set_scale_limit(GLfloat min_scale, GLfloat max_scale){this->min_scale = min_scale;this -> max_scale = max_scale;};
  int start_animation(GLuint animation, GLuint next_animation=0);
  int set_frame(GLint frame){this->frame=frame;animation_active = false; return 0;};
  //  void set_follow(GLuint follow){this -> follow = follow;};
  GLfloat get_width(){return (GLfloat)(ssheet->get_width())*scale;};
  GLfloat get_height(){return (GLfloat)(ssheet->get_height())*scale;};

  // GLfloat get_xpos(){return (this -> x);};
  // GLfloat get_ypos(){return (this -> y);};
  GLfloat get_direction(){return atan2(v_y,v_x)*180/M_PI;};
  GLfloat get_speed(){return hypot(v_x,v_y);};
  //tolua_end  
  void draw();//отрисовка
  decay_state think();//анимация, движение, и т.д.
  //tolua_begin
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

  GLuint animation,state;//номер анимации и фрейма
  GLuint animation_timer;//таймер переключения анимации
  GLuint next_animation;//следующая анимация
  //TODO: надо бы всё это забабахать в структуру
  
  GLuint decay_timer;//таймер исчезновения

  GLboolean animation_active;//флаг активности анимации
  GLboolean decay_active;//флаг активности таймера полного исчезновения
  GLfloat scale;
  
  GLboolean blur;

  Layer layer;

  CSprite* follow;
  CSpriteSheet* ssheet;//указатель на спрайтовый лист
  //tolua_end  
private:

  //  GLboolean destroy_tracking;

  friend class CSpriteManager;
  //tolua_begin
};

class CSpriteManager{
public:
  CSpriteManager();
  //tolua_end
  void think();
  void draw(Layer layer);
  //tolua_begin
  void add_sprite(CSprite* new_sprite);
  //  GLuint create_sprite(std::string spritesheet, Layer layer=LAYER_EMBLEM);
  
  void destroy_sprite(CSprite* sprite);
  //  CSprite* get_sprite(GLuint handle);
  GLboolean sprite_destroyed(CSprite* handle);
  //tolua_end
private:
  //  std::map<GLuint,CSprite*> collection;
  //  std::set<GLuint> destroyed_collection;
  //  GLuint free_handle;//минимальный свободный псевдоуказатель
  std::set<CSprite*> collection;
  //  CCollection<CSprite> collection;
}; //tolua_export

namespace game{
  //tolua_begin
  extern CSpriteManager* smanager;
  extern CSpriteSheetManager* ssmanager;
  //tolua_end
}

#endif
