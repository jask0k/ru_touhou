#ifndef _HERO_BULLET_HPP
#define _HERO_BULLET_HPP

#include "sprite.hpp"
#include "enemy.hpp"
#include "hero.hpp"
//tolua_begin
class CHeroBullet{
public:
  CSprite* sprite;
  CHeroBullet(GLuint proto_no, GLfloat speed, GLfloat angle=90., GLfloat xshift = 0, GLfloat yshift = 0);
  CHeroBullet(GLuint proto_no, GLfloat speed, GLfloat angle, CSprite* shift);
  ~CHeroBullet();
  //tolua_end
  GLboolean think();
private:
  void init(GLuint proto_no, GLfloat speed, GLfloat angle, CSprite* shift);  
  GLuint proto_no;
  friend class CHeroBulletManager;
  //tolua_begin
};

struct SBulletProto{
  std::string die_func;
  GLfloat damage;
  GLfloat scale;
  GLboolean animated;
  GLint frame_animation;
  GLfloat r,g,b,a;
};

class CHeroBulletManager{
public:
  CHeroBulletManager();
  void load_spritesheet(CSpriteSheet* ssheet);
  GLfloat collide(GLfloat x, GLfloat y, GLfloat hitbox_radius);//проверка столкновений с пулями объектов
  void add(CHeroBullet *new_member); 
  GLuint create_proto(std::string die_func, GLfloat damage, 
			     GLfloat scale, GLboolean animated, GLint frame_animation, 
			     GLfloat r=1.0, GLfloat g=1.0, GLfloat b=1.0, GLfloat a=1.0);
  void destroy(CHeroBullet* bullet);
  //tolua_end
  void think();
private:
  CSpriteSheet* spritesheet;
  std::set<CHeroBullet*> collection;
  std::vector<SBulletProto> proto_collection;
  //tolua_begin
};
//tolua_end

namespace game{
  extern CHeroBulletManager* hbmanager; //tolua_export
}
#endif
