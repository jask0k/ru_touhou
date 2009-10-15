#include "script.hpp"

#include <cstdarg>

#define declare_number(name) lua_pushnumber(level_state, name);lua_setglobal(level_state, #name)
#define declare_function(name) int name(lua_State* L)
#define bind_function(name) lua_register(level_state, #name, bind::name)
#define get_sprite(name) (game::smanager -> get_sprite(name))
#define get_bullet(name) (game::ebmanager -> get_bullet(name))


namespace script{
  //функция чтения скриптов из dat
  const char* reader(lua_State* L, void* filename, size_t* size);
  //формат вида "ifs", где каждый символ - тип переменной
  int parameters_parse(lua_State* L, std::string format, ...);
}

namespace bind{
//бинды
  declare_function(log);
  declare_function(wait);//Пауза на несколько кадров
  declare_function(thread_start);

  declare_function(engine_get_frame);
  declare_function(engine_god_mode);
  declare_function(engine_weapon_lockdown);

  declare_function(hero_x);
  declare_function(hero_y);
  declare_function(hero_sprite);

  declare_function(spritesheet_load);//загрузка спрайтового листа менеджером

  declare_function(enbullet_create_proto);
  declare_function(enbullet_create);
  declare_function(enbullet_create_target);
  declare_function(enbullet_create_hero);
  declare_function(enbullet_destroy);
  declare_function(enbullet_lock_on);
  declare_function(enbullet_lock_on_hero);
  declare_function(enbullet_stray);
  declare_function(enbullet_stop);

  declare_function(sprite_create);//создание спрайтов
  declare_function(sprite_destroy);
  declare_function(sprite_destroyed);
  declare_function(sprite_get_position);
  declare_function(sprite_get_vector);
  declare_function(sprite_get_angle);
  
  declare_function(sprite_set_position);//установка координат спрайта
  declare_function(sprite_set_speed);
  declare_function(sprite_set_angle);
  declare_function(sprite_set_tint);
  declare_function(sprite_set_alpha);
  declare_function(sprite_set_alpha_speed);
  declare_function(sprite_set_alpha_limit);
  declare_function(sprite_set_scale);
  declare_function(sprite_set_scale_speed);
  declare_function(sprite_set_scale_limit);
  declare_function(sprite_set_decay);
  declare_function(sprite_set_blur);
  declare_function(sprite_set_frame);
  declare_function(sprite_set_follow);
  declare_function(sprite_start_animation);

  declare_function(particle_set_colour);
  declare_function(particle_create_from);
  declare_function(particle_create_to);
  declare_function(particle_create_angle);
  
  declare_function(background_set_speed);
  declare_function(background_set_rotation);
  declare_function(background_set_rotation_speed);
  declare_function(background_set_fog_colour);
  declare_function(background_set_fog_density);

  declare_function(sound_create);
  declare_function(sound_destroy);
  declare_function(sound_play);

  declare_function(music_play);
}

int CScript::do_binds(){
  bind_function(wait);
  bind_function(log);
  bind_function(thread_start);

  bind_function(engine_get_frame);
  bind_function(engine_god_mode);
  bind_function(engine_weapon_lockdown);

  bind_function(spritesheet_load);

  bind_function(hero_x);
  bind_function(hero_y);
  bind_function(hero_sprite);

  bind_function(enbullet_create_proto);
  bind_function(enbullet_create);
  bind_function(enbullet_create_target);
  bind_function(enbullet_create_hero);
  bind_function(enbullet_destroy);
  bind_function(enbullet_lock_on);
  bind_function(enbullet_lock_on_hero);
  bind_function(enbullet_stray);
  bind_function(enbullet_stop);

  bind_function(sprite_create);
  bind_function(sprite_destroy);
  bind_function(sprite_destroyed);
  bind_function(sprite_get_position);
  bind_function(sprite_get_vector);
  bind_function(sprite_get_angle);
  
  bind_function(sprite_set_position);
  bind_function(sprite_set_speed);
  bind_function(sprite_set_angle);
  bind_function(sprite_set_tint);
  bind_function(sprite_set_alpha);
  bind_function(sprite_set_alpha_speed);
  bind_function(sprite_set_alpha_limit);
  bind_function(sprite_set_scale);
  bind_function(sprite_set_scale_speed);
  bind_function(sprite_set_scale_limit);
  bind_function(sprite_set_decay);
  bind_function(sprite_set_blur);
  bind_function(sprite_set_frame);
  bind_function(sprite_set_follow);
  bind_function(sprite_start_animation);

  bind_function(particle_set_colour);
  bind_function(particle_create_from);
  bind_function(particle_create_to);
  bind_function(particle_create_angle);

  bind_function(background_set_speed);
  bind_function(background_set_rotation);
  bind_function(background_set_rotation_speed);
  bind_function(background_set_fog_colour);
  bind_function(background_set_fog_density);

  bind_function(sound_create);
  bind_function(sound_destroy);
  bind_function(sound_play);

  bind_function(music_play);

  return 0;
}

int CScript::do_globals(){
  declare_number(GAME_FIELD_WIDTH);
  declare_number(GAME_FIELD_HEIGHT);
  declare_number(LAYER_BACKGROUND);
  declare_number(LAYER_ENEMY_BULLET);
  declare_number(LAYER_ENEMY);
  declare_number(LAYER_HERO_BULLET);
  declare_number(LAYER_HERO);
  declare_number(LAYER_EMBLEM);

  declare_number(CONTROL_BULLET);
  declare_number(CONTROL_SPRITE);
  return 0;
}

CScript::CScript():level_state(luaL_newstate()){
  luaL_openlibs(level_state);
  do_globals();
  do_binds();
  lua_newtable(level_state);
  lua_setglobal(level_state,"AI_table");
}

CScript::~CScript(){
  lua_close(level_state);
}

int CScript::load_script(std::string scriptname){
  std::string full_path=std::string("th_ru/")+scriptname+std::string(".luc");
  return lua_load(level_state,script::reader, (void*)full_path.c_str(), scriptname.c_str());
}

int CScript::run_script(std::string scriptname){
  load_script(scriptname);
  if (lua_pcall(level_state,0,0,0) == 0){
    return 0;
  } else {
#ifdef DEBUG
    std::string err_message(luaL_checklstring(level_state,1,NULL));
    luaL_where(level_state,0);
    std::string err_pos = lua_tolstring(level_state,-1,NULL);
    std::cerr <<"lua error: "<< err_pos << err_message << std::endl;
#endif
    return 1;
  }
}

int CScript::run_function(std::string funcname){
  lua_getfield(level_state, LUA_GLOBALSINDEX, funcname.c_str());
  if (lua_isfunction(level_state,-1))
    if (lua_pcall(level_state,0,0,0) != 0){
#ifdef DEBUG
      std::string err_message(luaL_checklstring(level_state,1,NULL));
      luaL_where(level_state,0);
      std::string err_pos = lua_tolstring(level_state,-1,NULL);

      std::cerr << "lua error while running function:" << err_pos << err_message << std::endl;
#endif
      return 1;
    }
  return 0;
}

GLboolean CScript::check_cond(lua_State* L, std::string cond){
  std::string query = std::string("return (") + cond + std::string(")");
  luaL_dostring(L,query.c_str());
  GLboolean result = lua_toboolean(L,-1);
  lua_pop(L,1);
  return result;
}

lua_State* CScript::create_AI_state(lua_State* L){
  if (lua_isfunction(L, -1) == 1 && lua_isstring(L, -2) == 1){
    //создаём тред АИ
    lua_State* state = lua_newthread(L);
    //переносим указатель на тред на дно стека, он нам пока не нужен
    lua_insert(L,1);
    //переносим указатель на функцию в стек треда АИ
    lua_xmove(L,state,1);

    //забираем условие выхода из стека
    std::string cond = std::string(luaL_checkstring(L,-1));
    //    std::cerr << cond << std::endl;
    //и удаляем его оттудова
    lua_pop(L,1);
    //смотрим, сколько лишних параметров у нас есть
    GLuint par_num = lua_gettop(L)-1;
    //    std::cerr << par_num << std::endl;
    //и суём их все в параметры треда
    lua_xmove(L,state,par_num);
    //создаём структуру, содержащую инфу про тред АИ
    AI_state st = {0,cond};
    //и засовываем её в коллекцию
    AI_states.insert(std::pair<lua_State*,AI_state>(state,st));
    lua_getglobal(L,"AI_table");
    lua_insert(L,1);
    std::ostringstream reader;
    reader << state; 
    //    std::cerr << state_name << std::endl;
    lua_setfield(L,1,reader.str().c_str());
    lua_resume(state,1);
    return state;
  }
  return NULL;
}

int CScript::destroy_AI_state(std::map<lua_State*,AI_state>::iterator position){
  std::ostringstream reader;
  reader << position->first; 
  lua_getglobal(level_state,"AI_table");
  lua_pushnil(level_state);
  lua_setfield(level_state,-2,reader.str().c_str());
  std::cerr << "destroing " << reader.str() << std::endl;
  AI_states.erase(position);
  return 0;
}

GLint CScript::get_integer(std::string var_name){
  lua_getglobal(level_state, var_name.c_str());
  return luaL_checkint(level_state,-1);
}
GLint CScript::get_integer(const char* var_name){
  lua_getglobal(level_state, var_name);
  return luaL_checkint(level_state,-1);
}
GLfloat CScript::get_number(std::string var_name){
  lua_getglobal(level_state, var_name.c_str());
  return luaL_checknumber(level_state,-1);
}
GLfloat CScript::get_number(const char* var_name){
  lua_getglobal(level_state, var_name);
  return luaL_checknumber(level_state,-1);
}
std::string CScript::get_string(std::string var_name){
  const char* value;
  lua_getglobal(level_state, var_name.c_str());
  value = luaL_checklstring(level_state,-1,NULL);
  return std::string(value);
}
std::string CScript::get_string(const char* var_name){
  const char* value;
  lua_getglobal(level_state, var_name);
  value = luaL_checklstring(level_state,-1,NULL);
  return std::string(value);
}

GLint CScript::set_integer(std::string var_name, GLint value){
  lua_pushinteger(level_state,value);
  lua_setglobal(level_state,var_name.c_str());
  return value;
}

GLint CScript::set_integer(const char* var_name, GLint value){
  lua_pushinteger(level_state,value);
  lua_setglobal(level_state,var_name);
  return value;
}

GLfloat CScript::set_number(std::string var_name, GLfloat value){
  lua_pushnumber(level_state,value);
  lua_setglobal(level_state,var_name.c_str());
  return value;
}

GLfloat CScript::set_number(const char* var_name, GLfloat value){
  lua_pushnumber(level_state,value);
  lua_setglobal(level_state,var_name);
  return value;
}

std::string CScript::set_string(std::string var_name, std::string value){
  lua_pushlstring(level_state, value.c_str(), value.size());
  lua_setglobal(level_state, var_name.c_str());
  return value;
}

std::string CScript::set_string(const char* var_name, std::string value){
  lua_pushlstring(level_state, value.c_str(), value.size());
  lua_setglobal(level_state, var_name);
  return value;
}


int CScript::think(){
  std::map<lua_State*,AI_state>::iterator i;
  std::map<lua_State*,AI_state>::iterator bad_handle;
  for (i = AI_states.begin(); i != AI_states.end();){
    bool cleanup=false;
    if (i -> second.timer > 0)
      --(i->second.timer);
    if (check_cond(i->first, i->second.quit_condition)){
      cleanup = true;
    }
    if (cleanup){
      bad_handle = i;
      ++i;
      if (destroy_AI_state(bad_handle) == -1){
#ifdef DEBUG
	std::cerr << "fixme:Could not destroy AI state!" << std::endl;
#endif
      }
      cleanup = false;
    }
    else if (i -> second.timer == 0){
// #ifdef DEBUG
//       std::cerr << "stack content '"<<i->first<<"'"<<std::endl;
//       int it;
//       for (it=1;it<=lua_gettop(i->first);++it)
// 	std::cerr << lua_typename(i->first, lua_type(i->first, it)) << std::endl;
//       std::cerr << "end!"<<std::endl;
//#endif
      int result = lua_resume(i->first,0);
// #ifdef DEBUG
//       std::cerr << "stack content '"<<i->first<<"'"<<std::endl;
//       //      int it;
//       for (it=1;it<=lua_gettop(i->first);++it)
// 	std::cerr << lua_typename(i->first, lua_type(i->first, it)) << std::endl;
//       std::cerr << "end!"<<std::endl;
// #endif
      if (result!=LUA_YIELD){
	//завершились с ошибкой или скрипт закончил выполнение
	bad_handle = i;
	++i;
	if (destroy_AI_state(bad_handle) == -1){
#ifdef DEBUG
	  std::cerr << "fixme:Could not destroy AI state!" << std::endl;
#endif
	}
#ifdef DEBUG
	if (result!=0){
	  std::cerr << "stack content '"<<i->first<<"'"<<std::endl;
	  int it;
	  for (it=1;it<=lua_gettop(i->first);++it)
	    std::cerr << lua_typename(i->first, lua_type(i->first, it)) << std::endl;
	  std::cerr << "end!"<<std::endl;
	  std::cerr << "error code:" << result<< std::endl;
	  const char* err_string = luaL_checklstring(i->first,-1,NULL);
	  std::cerr << "script error:" << err_string << std::endl;
	}
#endif
	
      } 
      else
	++i;
    } else ++i;
  }
  if (state.resume){
    lua_resume(level_state, 0);
    state.resume=false;
  }
  if (timer_active){
    if (--timer == 0){
      timer_active = false;
      if (lua_resume(level_state, 0) == 0)
	return 1;
    }
  }
  return 0;
}

int CScript::init_level(int level){
  std::ostringstream s;
  s << "level" << level;
  load_script(s.str());
  state.resume = true;
  return 0;
}

int CScript::set_timer(lua_State* state, GLuint timer){
  if (state == level_state){
    this -> timer = timer;
    this -> timer_active = true;
  }
  else
    AI_states[state].timer = timer; 
  return 0;
}

const char* script::reader(lua_State* L, void* filename, size_t* size){
  (void)L;//чтоб не было варнинга
  SDL_RWops* rwops;
  char* buf = (char*)calloc(1024*1024,sizeof(char));
  rwops=SDL_RWFromZZIP((char*)filename,"r");
  *size = SDL_RWread(rwops, buf, sizeof(char), 1024*1024);
  buf = (char*)realloc(buf,*size);
  return buf;
}

int script::parameters_parse(lua_State* L, std::string format, ...){
  enum {
    //    TYPE_VOID = ' ',
    TYPE_INTEGER = 'i',
    TYPE_FLOAT = 'f',
    TYPE_STRING = 's'
  };
  GLuint stack_size = lua_gettop(L);
  if (stack_size != format.size()){
#ifdef DEBUG
    luaL_where(L,0);
    std::string err_pos = lua_tolstring(L,-1,NULL);
    std::cerr << err_pos << "arguments error!" << std::endl;
#endif
    lua_pushstring(L,"Wrong arguments number!");
    lua_error(L);
  }
  va_list vl;
  va_start(vl,format);
  std::string::iterator it;
  unsigned int cur_var;
  for (cur_var=1,it=format.begin();it!=format.end();++it,++cur_var){
    switch(*it){
    case TYPE_INTEGER:{
      int* var = va_arg(vl,int*);
      *var = luaL_checkint(L,cur_var);
    }
      break;
    case TYPE_FLOAT:{
      float* var = va_arg(vl,float*);
      *var = (float)luaL_checknumber(L,cur_var);
    }
      break;
    case TYPE_STRING:{
      const char** var = va_arg(vl,const char**);
      *var = (luaL_checklstring(L,cur_var,NULL));
    }
      break;
    }
  }
  va_end(vl);
  return stack_size;
}

int bind::wait(lua_State* L){
  int timer;
  script::parameters_parse(L,"i",&timer);
  game::script->set_timer(L, timer);
  return lua_yield(L, 0);
}

int bind::log(lua_State* L){
  char* message;
  script::parameters_parse(L,"s",&message);
#ifdef DEBUG
  std::cerr << "Script says:" << message << std::endl;
#endif
  return 0;
}

//параметры: целое - хендл врага, и функция - функция вида func(handle)
int bind::thread_start(lua_State* L){
  lua_State* thread = game::script -> create_AI_state(L);
  if (thread!=NULL){
    //    lua_pushthread(thread);
    //    lua_xmove(thread,L,1);
    //return 1;
    return 0;
  }
  else{
#ifdef DEBUG
    std::cerr << "Could not create AI state!" << std::endl;
#endif
    return 0;
  }
}

int bind::engine_get_frame(lua_State* L){
  lua_Integer frame = game::engine->get_frame();
  lua_pushinteger(L,frame);
  return 1;
}

int bind::engine_god_mode(lua_State* L){
  GLuint god_timer;
  script::parameters_parse(L,"i",&god_timer);
  game::engine -> state.god_timer = god_timer;
  return 0;
}

int bind::engine_weapon_lockdown(lua_State* L){
  GLuint lockdown;
  script::parameters_parse(L,"i",&lockdown);
  game::engine -> state.lockdown = (lockdown != 0);
  return 0;
}

int bind::hero_x(lua_State* L){
  lua_Number x = game::hero->x;
  lua_pushnumber(L,x);
  return 1;
}

int bind::hero_y(lua_State* L){
  lua_Integer y = game::hero->y;
  lua_pushnumber(L,y);
  return 1;
}

int bind::hero_sprite(lua_State* L){
  lua_Number sprite = game::hero->sprite_no;
  lua_pushnumber(L,sprite);
  return 1;
}

int bind::spritesheet_load(lua_State* L){
  char* ssname;
  script::parameters_parse(L,"s",&ssname);

  game::ssmanager->load(ssname);
  return 0;
};

int bind::enbullet_create_proto(lua_State* L){
  char* spritesheet;
  GLint frame_animation;
  GLint animated;
  GLfloat scale;
  
  script::parameters_parse(L,"siif", &spritesheet, &frame_animation, &animated, &scale);
  GLuint handle = game::ebmanager->create_proto(spritesheet,frame_animation,(animated==1),scale);
  lua_pushinteger(L,handle);
  return 1;
}

int bind::enbullet_create(lua_State* L){
  GLint proto;
  GLfloat xpos, ypos;
  GLfloat speed, angle;
  
  script::parameters_parse(L,"iffff", &proto, &xpos, &ypos, &speed, &angle);
  GLuint handle = game::ebmanager->create_bullet(proto,xpos,ypos,speed,angle);
  lua_pushinteger(L,handle);
  return 1;
}

int bind::enbullet_create_target(lua_State* L){
  GLint proto;
  GLfloat xpos, ypos;
  GLfloat speed, xtarget, ytarget;
  GLfloat stray;
  
  script::parameters_parse(L,"iffffff", &proto, &xpos, &ypos, &speed, &xtarget, &ytarget, &stray);
  GLuint handle = game::ebmanager->create_bullet_aimed(proto,xpos,ypos,speed,
						       xtarget,ytarget,stray);
  lua_pushinteger(L,handle);
  return 1;
}

int bind::enbullet_create_hero(lua_State* L){
  GLint proto;
  GLfloat xpos, ypos;
  GLfloat speed;
  GLfloat stray;
  
  script::parameters_parse(L,"iffff", &proto, &xpos, &ypos, &speed, &stray);
  GLuint handle = game::ebmanager->create_bullet_aimed_hero(proto,xpos,ypos,speed,stray);
  lua_pushinteger(L,handle);
  return 1;
}

int bind::enbullet_destroy(lua_State* L){
  GLuint handle;

  script::parameters_parse(L, "i", &handle);
  game::ebmanager -> destroy_bullet(handle);
  return 0;
}

int bind::enbullet_lock_on(lua_State* L){
  GLuint handle;
  GLfloat posx, posy;
  GLfloat stray, speed;
  script::parameters_parse(L, "iffff", &handle, &posx, &posy, &stray, &speed);
  get_bullet(handle) -> lock_on(posx,posy,stray,speed);
  return 0;
}

int bind::enbullet_lock_on_hero(lua_State* L){
  GLuint handle;
  GLfloat stray, speed;
  script::parameters_parse(L, "iff", &handle, &stray, &speed);
  get_bullet(handle) -> lock_on_hero(stray,speed);
  return 0;
}

int bind::enbullet_stray(lua_State* L){
  GLuint handle;
  GLfloat angle;
  script::parameters_parse(L, "if", &handle, &angle);
  get_bullet(handle) -> stray(angle);
  return 0;
}

int bind::enbullet_stop(lua_State* L){
  GLuint handle;
  script::parameters_parse(L, "i", &handle);
  get_bullet(handle) -> stop();
  return 0;
}

int bind::sprite_create(lua_State* L){
  char* ssname;
  Layer layer;
  script::parameters_parse(L,"si",&ssname,&layer);
  
  GLuint sprite_handle = game::smanager->create_sprite(std::string(ssname), layer);
  lua_pushinteger(L,sprite_handle);
  return 1;
}

int bind::sprite_destroy(lua_State* L){
  GLuint sprite;
  script::parameters_parse(L,"i",&sprite);
  game::smanager -> destroy_sprite(sprite);
  return 0;
}

int bind::sprite_destroyed(lua_State* L){
  GLuint sprite;
  script::parameters_parse(L,"i",&sprite);
  int result = game::smanager -> sprite_destroyed(sprite);
  lua_pushboolean(L,result);
  return 1;
}

int bind::sprite_get_position(lua_State* L){
  GLuint sprite;
  script::parameters_parse(L,"i",&sprite);
  GLfloat pos_x = get_sprite(sprite) -> get_xpos();
  lua_pushnumber(L,pos_x);
  GLfloat pos_y = get_sprite(sprite) -> get_ypos();
  lua_pushnumber(L,pos_y);
  return 2;
}

int bind::sprite_get_vector(lua_State* L){
  GLuint sprite;
  script::parameters_parse(L,"i",&sprite);
  return 0;//ДОПИСАТ!
}

int bind::sprite_get_angle(lua_State* L){
  GLuint sprite;
  script::parameters_parse(L,"i",&sprite);
  GLfloat dir = get_sprite(sprite) -> get_direction();
  GLfloat speed = get_sprite(sprite) -> get_speed();
  lua_pushnumber(L,dir);
  lua_pushnumber(L,speed);
  return 2;
}

int bind::sprite_set_position(lua_State* L){
  GLfloat x,y,rot;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&x,&y,&rot);

  get_sprite(sprite) -> set_position(x,y,rot);
  return 0;
}

int bind::sprite_set_angle(lua_State* L){
  GLfloat r,a;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&r,&a);
  get_sprite(sprite) -> set_angle(r,a);
  return 0;
}

int bind::sprite_set_speed(lua_State* L){
  GLfloat vx,vy,rot;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&vx,&vy,&rot);
  get_sprite(sprite) -> set_speed(vx,vy,rot);
  return 0;
}

int bind::sprite_set_tint(lua_State* L){
  GLfloat red,green,blue;
  GLuint sprite;
  script::parameters_parse(L,"ifff",&sprite,&red,&green,&blue);
  get_sprite(sprite) -> set_tint(red,green,blue);
  return 0;
}

int bind::sprite_set_alpha(lua_State* L){
  GLfloat amount;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&amount);
  get_sprite(sprite) -> set_alpha(amount);
  return 0;
}

int bind::sprite_set_alpha_speed(lua_State* L){
  GLfloat amount;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&amount);
  get_sprite(sprite) -> set_alpha_speed(amount);
  return 0;
}

int bind::sprite_set_alpha_limit(lua_State* L){
  GLfloat min,max;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&min,&max);
  get_sprite(sprite) -> set_alpha_limit(min,max);
  return 0;
}

int bind::sprite_set_scale(lua_State* L){
  GLfloat scale;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&scale);
  get_sprite(sprite) -> set_scale(scale);
  return 0;
}

int bind::sprite_set_scale_speed(lua_State* L){
  GLfloat v_scale;
  GLuint sprite;
  script::parameters_parse(L,"if",&sprite,&v_scale);
  get_sprite(sprite) -> set_scale_speed(v_scale);
  return 0;
}

int bind::sprite_set_scale_limit(lua_State* L){
  GLfloat min,max;
  GLuint sprite;
  script::parameters_parse(L,"iff",&sprite,&min,&max);
  get_sprite(sprite) -> set_scale_limit(min,max);
  return 0;
}

int bind::sprite_set_decay(lua_State* L){
  GLint time;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&time);
  get_sprite(sprite) -> set_decay(time);
  return 0;
}
int bind::sprite_set_blur(lua_State* L){
  GLint blur;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&blur);
  get_sprite(sprite) -> set_blur( blur!=0 );
  return 0;
}

int bind::sprite_set_frame(lua_State* L){
  GLint frame;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&frame);
  get_sprite(sprite) -> set_frame(frame);
  return 0;
}
int bind::sprite_start_animation(lua_State* L){
  GLint animation,next_animation;
  GLuint sprite;
  script::parameters_parse(L,"iii",&sprite,&animation,&next_animation);
  get_sprite(sprite) -> start_animation(animation,next_animation);
  return 0;
}

int bind::sprite_set_follow(lua_State* L){
  GLuint follow;
  GLuint sprite;
  script::parameters_parse(L,"ii",&sprite,&follow);
  get_sprite(sprite) -> set_follow(follow);
  return 0;
}

int bind::particle_set_colour(lua_State* L){
  GLfloat r,g,b,a;
  script::parameters_parse(L,"ffff",&r,&g,&b,&a);
  game::pmanager -> set_colour(r,g,b,a);
  return 0;
}

int bind::particle_create_from(lua_State* L){
  GLfloat posx,posy;
  GLuint decay;
  script::parameters_parse(L,"ffi",&posx,&posy,&decay);
  game::pmanager -> create_from(posx,posy,decay);
  return 0;
}

int bind::particle_create_to(lua_State* L){
  GLfloat posx,posy;
  GLuint decay;
  script::parameters_parse(L,"ffi",&posx,&posy,&decay);
  game::pmanager -> create_to(posx,posy,decay);
  return 0;
}

int bind::particle_create_angle(lua_State* L){
  GLfloat posx,posy,angle;
  GLuint decay;
  script::parameters_parse(L,"ffif",&posx,&posy,&decay,&angle);
  game::pmanager -> create_angle(posx,posy,decay,angle);
  return 0;
}

int bind::background_set_speed(lua_State* L){
  GLfloat vy,vx;
  script::parameters_parse(L,"ff",&vy,&vx);
  game::background -> set_speed(vy,vx);
  return 0;
}

int bind::background_set_rotation(lua_State* L){
  GLfloat rx,ry,rz;
  script::parameters_parse(L,"fff",&rx,&ry,&rz);
  game::background -> set_rotation(rx,ry,rz);
  return 0;
}
  
int bind::background_set_rotation_speed(lua_State* L){
  GLfloat rvx,rvy,rvz;
  script::parameters_parse(L,"fff",&rvx,&rvy,&rvz);
  game::background -> set_rotation(rvx,rvy,rvz);
  return 0;
}

int bind::background_set_fog_colour(lua_State* L){
  GLfloat r,g,b,a;
  script::parameters_parse(L,"ffff",&r,&g,&b,&a);
  game::background -> set_fog_colour(r,g,b,a);
  return 0;
}

int bind::background_set_fog_density(lua_State* L){
  GLfloat density,vfog;
  script::parameters_parse(L,"ff",&density,&vfog);
  game::background -> set_fog_density(density,vfog);
  return 0;
}

int bind::sound_create(lua_State* L){
  char* filename;
  script::parameters_parse(L,"s",&filename);

  std::string full_path = std::string("th_ru/")+filename;
  SDL_RWops* file=SDL_RWFromZZIP(full_path.c_str(), "r");
  int sound_handle = game::boom_box->create_sound(file);
  if(file) SDL_RWclose(file);
  lua_pushinteger(L,sound_handle);
  return 1;
}

int bind::sound_destroy(lua_State* L){
  int sound_handle;
  script::parameters_parse(L,"i",&sound_handle);
  game::boom_box -> destroy_sound(sound_handle);
  return 0;
}

int bind::sound_play(lua_State* L){
  int sound_handle;
  script::parameters_parse(L,"i",&sound_handle);
  game::boom_box -> play_sound(sound_handle);
  return 0;
}

int bind::music_play(lua_State* L){
  char* filename;
  script::parameters_parse(L,"s",&filename);

  std::string full_path = std::string("th_ru/")+filename;
  SDL_RWops* file=SDL_RWFromZZIP(full_path.c_str(), "r");
  game::boom_box -> play_music(file);
  if(file) SDL_RWclose(file);
  return 0;
}

