#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include <lua.hpp>


class CScript{
private:
  CEngine* engine;
  lua_State* level_state;
  int do_binds();
  //бинды
  int sprite_create (lua_State L);
  int sprite_set_position(lua_State L);
public:
  int load_script(char* scriptname);
  CScript(CEngine* engine);
  ~CScript();
};

#endif
