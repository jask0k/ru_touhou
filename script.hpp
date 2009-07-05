#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include <lua.hpp>


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
  //бинды
  int sprite_create (lua_State* L);
  int sprite_set_position(lua_State* L);
  int sprite_set_speed(lua_State* L);
  int sprite_set_angle(lua_State* L);
public:
  int wait(lua_State* L);
  int load_script(std::string scriptname);
  int think();
  int init_level(int level);
  CScript();
  ~CScript();
};


#endif
