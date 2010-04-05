#ifndef _SCRIPT_HPP
#define _SCRIPT_HPP
class CEngine;


#include "game.hpp"
#include "hero.hpp"
#include "back.hpp"
#include "enemy_bullet.hpp"
#include "text.hpp"
#include <lua.hpp>
#include "SDL_rwops_zzip.h"
#include <string>

//ЧТО ЭТОАААААААА?
enum control_type{
  CONTROL_BULLET,
  CONTROL_SPRITE
};

struct AI_state{
  //таймер, который позволяет приостанавливать тред на несколько кадров
  GLuint timer;
  //кондишен завершения и удаления треда
  std::string quit_condition;
  //кондишен продолжения треда после остановки
  std::string wait_condition;
};

class CScript{
private:
  //Основной тред
  lua_State* level_state;
  //  CEngine* engine;
  //таймер основного треда
  GLuint timer;
  GLboolean timer_active;
  //кондишен продолжения треда
  std::string cond;
  
  struct{
    GLboolean resume;
  } state;
  //коллекция состояний тредов
  //(в связи со злобным сборщиком мусора луа, продублирована в главном треде луа)
  std::map<lua_State*,AI_state> AI_states;

  int do_binds();
  int do_globals();
public:
  //загрузка скрипта из дата
  int load_script(std::string scriptname);
  //запуск скрипта из дата
  int run_script(std::string scriptname);
  //проверка условия
  GLboolean check_cond(lua_State* L, std::string cond);
  //запуск функции
  int run_function(std::string funcname);
  //запуск функции с целочисленным параметром
  //TODO: УБРАТЬ ЭТОТ МЕРЗКИЙ ХАК И НАПИСАТЬ НОРМАЛЬНЫЙ МЕТОД, С ВАРИАЦИЕЙ ТИПОВ ПАРАМЕТРОВ
  int run_function(std::string funcname, GLuint parameter);
  //создние треда с состоянием
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
  //проверяем состояния тредов, запускаем выполнение тех, кому нужно выполнятся
  int think();
  //запуск скрипта уровня
  int init_level(int level);
  //приостановка треда на несколько кадров
  int set_timer(lua_State* state, GLuint timer);
  //приостановка треда до выполнения условия
  int set_cond(lua_State* state, std::string cond);
  //  int set_cond(lua_State* state, const char* cond);
  CScript();
  ~CScript();
};

namespace game{
  extern CScript* script;
}
#endif
