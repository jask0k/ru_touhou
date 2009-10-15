#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include "hero.hpp"
#include "back.hpp"
#include "enemy_bullet.hpp"
#include <lua.hpp>
#include "SDL_rwops_zzip.h"
#include <string>

enum control_type{
  CONTROL_BULLET,
  CONTROL_SPRITE
};

struct AI_state{
  GLuint timer;
  //  GLuint handle;
  //  control_type type;
  std::string quit_condition;
};

class CScript{
private:
  lua_State* level_state;
  //  CEngine* engine;
  
  GLuint timer;
  GLboolean timer_active;
  
  struct{
    GLboolean resume;
  } state;

  std::map<lua_State*,AI_state> AI_states;

  int do_binds();
  int do_globals();
public:
  int load_script(std::string scriptname);
  int run_script(std::string scriptname);
  GLboolean check_cond(lua_State* L, std::string cond);
  int run_function(std::string funcname);
  lua_State* create_AI_state(lua_State* L);
  int destroy_AI_state(std::map<lua_State*,AI_state>::iterator position);
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
  int set_timer(lua_State* state, GLuint timer);
  CScript();
  ~CScript();
};

namespace game{
  extern CScript* script;
}
#endif
