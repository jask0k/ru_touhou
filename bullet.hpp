#ifndef _BULLET_HPP
#define _BULLET_HPP
#include <GL/gl.h>


#define GRAZE_MISS 0 //пуля промахнулась
#define GRAZE_WOUND 1 //пуля загрейзила цель
#define GRAZE_KILL 2 //пуля попала в цель

class CEnemyBullet{
private:
  GLfloat x,y;//координаты пули
  GLfloat vx,vy;//скорость по х и по у
  GLfloat rotation;//угол, под которым летит пуля
  GLfloat hitbox_size;//размер хитбокса круглой пули
  bool grazed;//отмечаем пули, которые погрейзили героя
  bool speed_set;//нужно будет для отладки скриптов
public:
  CEnemyBullet(GLfloat x0, GLfloat y0, GLfloat size);//инициализация
  GLint think();//движение пули по прямой, будет вызываться каждый кадр
  GLint graze(GLfloat target_x, GLfloat target_y, GLfloat target_size, GLfloat target_graze_size);//грейзим цель, 
  //и говорим, погрейзили или нет
  GLint set_target(GLfloat target_x, GLfloat target_y, GLfloat bullet_velocity);//наводим пулю на цель
  GLint set_angle(GLfloat angle, GLfloat velocity);//устанавливаем угол движения пули
  GLint rotate(GLfloat angle, GLfloat velocity);//поворачиваем пулю
};
#endif
