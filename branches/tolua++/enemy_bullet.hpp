#ifndef _ENEMY_BULLET_HPP
#define _ENEMY_BULLET_HPP

#include <map>
#include <cmath>

#include "script.hpp"
#include "sprite.hpp"
#include "config.hpp"
#include "particles.hpp"
//tolua_begin
#define BULLET_KILL 2
#define BULLET_GRAZE 1
#define BULLET_MISS 0
#define BULLET_DESTROYED -1

class CEnemyBullet{
public:
  CEnemyBullet(CSprite* sprite, GLfloat posx, GLfloat posy, GLfloat angle, GLfloat speed, GLuint proto_no);
  ~CEnemyBullet();
  //tolua_end
  //проверка на грейз/килл
  int graze();
  //tolua_begin
  //разворачиваем пулю в сторону заданой точки с заданым отклонением
  int lock_on(GLfloat posx, GLfloat posy, GLfloat stray, GLfloat speed);
  //разворачиваем пулю в сторону героя
  int lock_on_hero(GLfloat stray, GLfloat speed);
  //поворачиваем пулю
  int stray(GLfloat angle);
  //замораживание пули, типа как в матрице
  int stop();
  //tolua_end
private:
  //указатель на спрайт
  CSprite* sprite;
  //прогрейзился ли персонаж этой пулей?
  GLboolean grazed;
  //ускорение пули
  GLfloat acceleration;
  //  //является ли пуля управляемой скриптом
  //  GLboolean managed;
  GLuint proto_no;
  friend class CEnemyBulletManager;
  //tolua_begin
};

//Прототип пули
struct SEnBulletProto{
  CSpriteSheet* spritesheet;
  std::string die_func;
  GLfloat scale;
  GLboolean animated;
  GLint frame_animation;
  GLfloat r,g,b,a;
};

class CEnemyBulletManager{
public:
  CEnemyBulletManager();
  //Создание прототипа пули
  GLuint create_proto(CSpriteSheet* spritesheet, GLint frame_animation, GLboolean animated, GLfloat scale, std::string& die_func);
  //Изменение цвета прототипа
  void set_proto_tint(GLuint handle, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
  //Создание пули
  CEnemyBullet* create_bullet(GLuint proto, GLfloat xpos, GLfloat ypos, GLfloat speed, GLfloat angle);
  //Создание нацеленой пули
  CEnemyBullet* create_bullet_aimed(GLuint proto, GLfloat xpos, GLfloat ypos, GLfloat speed,
		       GLfloat xtarget, GLfloat ytarget, GLfloat stray);
  //Создание пули, нацеленой в героя
  CEnemyBullet* create_bullet_aimed_hero(GLuint proto, GLfloat xpos, GLfloat ypos, GLfloat speed, GLfloat stray);
  //Уничтожение заданой пули
  void destroy_bullet(CEnemyBullet* handle);
  //Уничтожение пуль, попавших в круг
  GLuint destroy_bullets_circle(GLfloat x, GLfloat y, GLfloat r);
  //Уничтожение пуль, попавших в прямоугольник
  GLuint destroy_bullets_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
  //Уничтожение всех пуль на экране
  GLuint destroy_bullets_all();
  //Возвращает указатель на пулю по хендлу
  //  CEnemyBullet* get_bullet(GLuint handle);
  //Проверяет, была ли уничтожена пуля с заданым хендлом
  GLboolean bullet_destroyed(CEnemyBullet* handle);
  //tolua_end
  //Пули думают и грейзят
  void think();
private:
  //  std::string spritesheet;
  ////Первый свободный хендл
  //  GLuint free_handle;
  //Коллекция пуль
  std::set<CEnemyBullet*> collection;
  //Коллекция прототипов
  std::vector<SEnBulletProto> proto_collection;
  //  std::set<GLuint> destroyed_collection;
}; //tolua_export

//удобное сокращение имени типа для циклов
typedef std::set<CEnemyBullet*>::iterator bullet_it;

namespace game{
  extern CEnemyBulletManager* ebmanager; //tolua_export
}
#endif
