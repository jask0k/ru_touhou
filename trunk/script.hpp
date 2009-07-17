#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include <lua.hpp>
#include "SDL_rwops_zzip.h"


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
