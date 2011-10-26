#ifndef _VARIABLES_HPP
#define _VARIABLES_HPP

#include "text.hpp" 
#include <map>
#include <sstream>
//tolua_begin
class CVariable{
public:
  CVariable(std::string name, GLuint value, GLuint default_speed=0, int font=-1, GLint x=0, GLint y=0, text_layer layer=LAYER_PANEL);
  ~CVariable();
  GLuint inc(int increment=1);
  GLuint dec(int decrement=1);
  GLuint set(GLuint value);
  GLuint get();
  GLuint slowly_inc(int increment, int speed=0, int frame_pause=0);
  GLuint slowly_dec(int increment, int speed=0, int frame_pause=0);
  GLuint default_speed;
  //tolua_end
  void think();//обновление лабела, изменение значения
  void update_label();
private:
  CLabel* display;
  const char* format;
  GLuint value;
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
  void add_variable(CVariable* pointer, std::string& name);
  void del_variable(CVariable* pointer);
  void think();
private:
  std::map<std::string,CVariable*> collection;
};

namespace game{
  extern CVariableManager *vmanager;
};

#endif
