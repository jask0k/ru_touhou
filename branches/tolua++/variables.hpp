#ifndef _VARIABLES_HPP
#define _VARIABLES_HPP

#include "text.hpp" 
//tolua_begin
class CVariable{
public:
  CVariable(std::string name, GLuint value, GLint default_speed);
  ~CVariable();
  GLuint inc(int increment=1);
  GLuint dec(int decrement=1);
  GLuint set(GLuint value);
  GLuint get();
  GLuint slowly_inc(int increment, int speed, int frame_pause=0);
  GLuint slowly_dec(int increment, int speed, int frame_pause=0);
  int default_speed;
  //tolua_end
  void think();//обновление лабела, изменение значения
private:
  CLabel* display;
  const char* format;
  int value;
  int nextvalue;
  //  int minvalue;
  int speed;
  int frames_pause;
  int pause_counter;
  //tolua_begin
};
//tolua_end

class CVariableManager{
public:
  void add_variable(CVariable*);
  void think();
private:
  std::set<CVariable*> collection;
};

namespace game{
  extern CVariableManager *vmanager;
};

#endif
