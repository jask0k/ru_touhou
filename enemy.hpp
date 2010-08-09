#ifndef _ENEMY_HPP
#define _ENEMY_HPP

#include <map>
#include <string>

#include "collection.hpp"
#include "enemy_bullet.hpp"
#include "sprite.hpp"

struct SEnemyProto{
  std::string sheetname;
  GLuint animation;
  GLfloat max_hp;
  GLuint bullet_proto;
  std::string die_func;
};

class CEnemy{
private:
  GLuint sprite_no;
  GLuint proto_no;
  GLuint bullet_proto;
  GLfloat hp;
  
  //  GLboolean managed;
public:
  CEnemy(GLuint proto, GLuint bullet_proto, GLuint sprite, GLfloat max_hp);
  GLboolean think();
  GLboolean damage(GLfloat amount);
  GLfloat get_x();
  GLfloat get_y();
  GLuint get_sprite_handle();
  GLuint shoot_at(GLfloat x, GLfloat y, GLfloat speed);
  GLuint shoot_at_hero(GLfloat stray, GLfloat speed);
  void set_speed(GLfloat vx, GLfloat vy, GLfloat vr=0);
  void set_animation(GLuint animation);
  friend class CEnemyManager;
};

class CEnemyManager{
private:
  CCollection<CEnemy> collection;
  std::vector<SEnemyProto> proto_collection;
  //  std::set<GLuint> destroyed_collection;
  //  GLuint free_handle;
public:
  CEnemyManager();
  GLuint create_enemy_proto(std::string sheetname, GLuint animation, GLfloat max_hp, GLuint bullet_proto, std::string die_func);
  GLuint create_enemy(GLuint proto, GLfloat x, GLfloat y);
  GLuint destroy_enemy(GLuint handle);
  GLboolean enemy_destroyed(GLuint handle);
  //сплеш-дамаг, дамаг от круглых пуль героя
  GLboolean damage_circle(GLfloat x, GLfloat y, GLfloat r, GLfloat amount);
  //дамаг по линии
  GLboolean damage_line(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat amount);
  //дамаг прямоугольником - марисолазеры
  GLboolean damage_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat amount);
  //дамаг сразу всем, типа бомбы
  GLboolean damage_all(GLfloat amount);
  void think();
};

namespace game{
  extern CEnemyManager* enmanager;
}

#endif
