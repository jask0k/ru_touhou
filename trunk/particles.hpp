#ifndef _PARTICLES_HPP
#define _PARTICLES_HPP

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <SDL_opengl.h>

#include "config.hpp"

#define PARTICLE_GOOD 0
#define PARTICLE_DESTROYED -1

#define DEBUG_SIZE 2

struct RGBAcolour{
  GLfloat r,g,b,a;
};

class CParticle{
private:
  //  //Размер частицы
  //  GLfloat size;
  //Цвет
  struct RGBAcolour colour;
  //Вектор скорости
  GLfloat vx,vy;
  //Координаты
  GLfloat x,y;
  //Оставшееся время
  GLuint time_left;
public:
  //Отрисовка
  void draw();
  //Перемещение
  int think();
  //Конструктор
  CParticle(struct RGBAcolour colour,
	    GLfloat x, GLfloat y, GLfloat vx, GLfloat vy, GLuint time_left);
  friend class CParticleManager;
};

class CParticleManager{
private:
  //Цвет, который будет придаваться созданным частицам
  struct RGBAcolour current_colour;
  //Коллекция частиц
  std::vector<CParticle*> collection;

public:
  //конструктор
  CParticleManager();
  //отрисовка всех частиц
  void draw();
  //Перемещение и уничтожение сгнивших частиц
  void think();
  //установка цвета следующих частиц
  void set_colour(struct RGBAcolour colour);
  //установка цвета следующих частиц(поцветовая версия)
  void set_colour(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
  //Создание рандомной частицы, убегающей от точки
  void create_from(GLfloat posx, GLfloat posy, GLuint decay);
  //Создание рандомной частицы, прибегающей на точку
  void create_to(GLfloat posx, GLfloat posy, GLuint decay);
  //создание рандомной частицы, летящей под определённым углом
  void create_angle(GLfloat posx, GLfloat posy, GLuint decay, GLfloat angle);
};

namespace game{
  extern CParticleManager* pmanager;
};
#endif
