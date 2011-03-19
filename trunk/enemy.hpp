#ifndef _ENEMY_HPP
#define _ENEMY_HPP

#include <map>
#include <set>
#include <string>

#include "enemy_bullet.hpp"
#include "sprite.hpp"

#include "collection.hpp"
class CEnemyBullet;
//tolua_begin
struct SEnemyProto{
  CSpriteSheet* sheetname;
  GLuint animation;
  GLfloat max_hp;
  GLuint bullet_proto;
  std::string die_func;
};

class CEnemy{
  //tolua_end
private:
  GLuint proto_no;
  GLuint bullet_proto;
  GLfloat hp;
  CSprite* sprite;
  
  //  GLboolean managed;
public:
  CEnemy(GLuint proto, GLuint bullet_proto, CSprite* sprite, GLfloat max_hp);//tolua_export
  ~CEnemy();
  GLboolean think();
  //tolua_begin
  GLboolean damage(GLfloat amount);
  GLfloat get_x();
  GLfloat get_y();
  //  GLuint get_sprite_handle();
  CEnemyBullet* shoot_at(GLfloat x, GLfloat y, GLfloat speed);
  CEnemyBullet* shoot_at_hero(GLfloat speed, GLfloat stray = 0);
  CEnemyBullet* shoot(GLfloat speed, GLfloat angle);
  void set_speed(GLfloat vx, GLfloat vy, GLfloat vr=0);
  //  void set_animation(GLuint animation);
  void start_animation(GLuint animation);
  //tolua_end
  friend class CEnemyManager;
  //tolua_begin
};

class CEnemyManager{
  //tolua_end
private:
  std::set<CEnemy*> collection;
  std::vector<SEnemyProto> proto_collection;
  //  std::set<GLuint> destroyed_collection;
  //  GLuint free_handle;
public:
  //tolua_begin
  CEnemyManager();
  GLuint create_enemy_proto(CSpriteSheet* sheetname, GLuint animation, GLfloat max_hp, GLuint bullet_proto, std::string die_func);
  CEnemy* create_enemy(GLuint proto, GLfloat x, GLfloat y);
  void destroy_enemy(CEnemy* handle);
  GLboolean enemy_destroyed(CEnemy* handle);
  //сплеш-дамаг, дамаг от круглых пуль героя
  void damage_circle(GLfloat x, GLfloat y, GLfloat r, GLfloat amount);
  //дамаг лучом с возможностью остановки на ближайшем к началу луча враге
  void damage_ray(GLfloat x, GLfloat y, GLfloat angle, GLfloat amount, GLboolean stop=false);
  //дамаг прямоугольником - марисолазеры
  void damage_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat amount);
  //дамаг сразу всем, типа бомбы
  void damage_all(GLfloat amount);
  //tolua_end
  void think();
};//tolua_export

namespace game{
  extern CEnemyManager* enmanager;//tolua_export
}
#endif
