#ifndef _ENEMY_BULLET_HPP
#define _ENEMY_BULLET_HPP

#include "script.hpp"
#include "sprite.hpp"
#include "config.hpp"
#include "particles.hpp"
#include <map>
#include <cmath>

#define BULLET_KILL 2
#define BULLET_GRAZE 1
#define BULLET_MISS 0
#define BULLET_DESTROYED -1

class CEnemyBullet{
public:
  CEnemyBullet(GLuint sprite_no, GLfloat posx, GLfloat posy, GLfloat angle, GLfloat speed);
  int graze();
  int lock_on(GLfloat posx, GLfloat posy, GLfloat stray, GLfloat speed);
  int lock_on_hero(GLfloat stray, GLfloat speed);
  int stray(GLfloat angle);
  int stop();
private:
  GLuint sprite_no;
  GLboolean grazed;
  GLfloat acceleration;
  friend class CEnemyBulletManager;
};

class CEnemyBulletManager{
public:
  CEnemyBulletManager(std::string spritesheet);
  GLuint create_bullet(GLint frame, GLfloat xpos, GLfloat ypos, GLfloat speed, GLfloat angle);
  GLuint create_bullet_aimed(GLint frame, GLfloat xpos, GLfloat ypos, GLfloat speed,
		       GLfloat xtarget, GLfloat ytarget, GLfloat stray);
  GLuint create_bullet_aimed_hero(GLint frame, GLfloat xpos, GLfloat ypos, GLfloat speed, GLfloat stray);
  GLuint destroy_bullet(GLuint handle);
  CEnemyBullet* get_bullet(GLuint handle);
  void think();
private:
  std::string spritesheet;
  GLuint free_handle;
  std::map<GLuint,CEnemyBullet*> collection;
};

namespace game{
  extern CEnemyBulletManager* ebmanager;
}
#endif
