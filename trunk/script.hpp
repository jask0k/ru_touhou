#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include "hero.hpp"
#include <lua.hpp>
#include "SDL_rwops_zzip.h"
#include <string>


class CScript{
private:
  CEngine* engine;
  lua_State* level_state;
  
  GLuint timer;
  GLboolean timer_active;
  
  struct{
    GLboolean resume;
  } state;

  int do_binds();
  int do_globals();
public:
  int load_script(std::string scriptname);
  int run_script(std::string scriptname);
  int run_function(std::string funcname);
  GLint get_integer(std::string var_name);
  GLint get_integer(const char* var_name);
  GLfloat get_number(std::string var_name);
  GLfloat get_number(const char* var_name);
  std::string get_string(std::string var_name);
  std::string get_string(const char* var_name);
  GLint set_integer(std::string var_name, GLint value);
  GLint set_integer(const char* var_name, GLint value);
  GLfloat set_number(std::string var_name, GLfloat value);
  GLfloat set_number(const char* var_name, GLfloat value);
  std::string set_string(std::string var_name, std::string value);
  std::string set_string(const char* var_name, std::string value);
  int think();
  int init_level(int level);
  int set_timer(GLuint timer);
  CScript();
  ~CScript();
};

namespace game{
  extern CScript* script;
}
#endif
