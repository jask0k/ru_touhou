#include "enemy_bullet.hpp"
 
CEnemyBullet::CEnemyBullet(GLuint sprite_no, GLfloat posx, GLfloat posy, 
			   GLfloat angle, GLfloat speed, GLuint proto):
  sprite_no(sprite_no), grazed(false), acceleration(0.0f), managed(false), proto_no(proto){
  CSprite* sprite_handle = game::smanager->get_sprite(sprite_no);
  sprite_handle -> set_position(posx, posy);
  sprite_handle -> set_angle(speed,angle);
}

int CEnemyBullet::graze(){
  GLfloat hero_x = game::hero -> x;
  GLfloat hero_y = game::hero -> y;
  CSprite* sprite_handle = game::smanager -> get_sprite(this->sprite_no);
  if (sprite_handle == NULL){
    return BULLET_DESTROYED;
  }
  GLfloat bullet_x = sprite_handle -> get_xpos();
  GLfloat bullet_y = sprite_handle -> get_ypos();
  GLfloat bullet_hitbox = (sprite_handle -> get_width()) * .3f;
  GLfloat hero_grazebox = (game::hero -> get_height())/2;
  GLfloat length = hypot(bullet_x - hero_x, bullet_y - hero_y);
  GLfloat angle = atan2( hero_x - bullet_x, hero_y - bullet_y)*180/M_PI;
  
  if ((game::engine -> state.god_timer) == 0){
    
    if (length <= bullet_hitbox + 3){
      return BULLET_KILL;
    } 
    else{
      if ((length <= bullet_hitbox + hero_grazebox)&& !(this -> grazed)){
	game::pmanager -> set_colour(1.f,0.f,0.f,.5f);
	game::pmanager -> create_angle(hero_x, hero_y, 30, angle);
	this -> grazed = true;
	return BULLET_GRAZE;
      }
    }
  }
  return BULLET_MISS;
}

int CEnemyBullet::lock_on(GLfloat posx, GLfloat posy, GLfloat stray, GLfloat speed){
  CSprite* sprite_handle = game::smanager -> get_sprite(this->sprite_no);
  GLfloat bullet_x = sprite_handle -> get_xpos();
  GLfloat bullet_y = sprite_handle -> get_ypos();
  if ((posx != bullet_x)||(posy != bullet_y)){//смотрим, отличаются ли координаты пули и игрока
    GLfloat angle = atan2(posy - bullet_y, posx - bullet_x)*180/M_PI+stray;
    sprite_handle -> set_angle(speed,angle);
    return 0;
  }
  return 1;
}

int CEnemyBullet::lock_on_hero(GLfloat stray, GLfloat speed){
  return this -> lock_on(game::hero -> x, game::hero -> y, stray, speed);
}

int CEnemyBullet::stop(){
  CSprite* sprite_handle = game::smanager -> get_sprite(this->sprite_no);
  if (sprite_handle == NULL)
    return 1;
  sprite_handle -> set_speed(0.f,0.f,0.f);
  return 0;
}

int CEnemyBullet::stray(GLfloat angle){
  CSprite* sprite = game::smanager -> get_sprite(this->sprite_no);
  if (sprite == NULL)
    return 1;
  GLfloat direction = sprite -> get_direction();
  GLfloat speed = sprite -> get_speed();
  sprite -> set_angle(speed,direction+angle);
  return 0;
}

CEnemyBulletManager::CEnemyBulletManager():free_handle(1){}

GLuint CEnemyBulletManager::create_proto(std::string& spritesheet, GLint frame_animation, GLboolean animated, GLfloat scale, std::string& die_func){
  SEnBulletProto proto={spritesheet, die_func, scale, animated, frame_animation,1.f,1.f,1.f,1.f};
  proto_collection.push_back(proto);
  return proto_collection.size()-1;
}

void CEnemyBulletManager::set_proto_tint(GLuint handle, GLfloat r, GLfloat g, GLfloat b, GLfloat a){
  proto_collection[handle].r = r;
  proto_collection[handle].g = g;
  proto_collection[handle].b = b;
  proto_collection[handle].a = a;
}

GLuint CEnemyBulletManager::create_bullet(GLuint proto, GLfloat xpos, GLfloat ypos, 
					  GLfloat speed, GLfloat angle){
  if (proto>=proto_collection.size()){
    throw "incorrect proto";
  }
  GLuint sprite_num = game::smanager -> create_sprite(proto_collection[proto].spritesheet, LAYER_ENEMY_BULLET);
  CEnemyBullet* bullet = new CEnemyBullet(sprite_num, xpos, ypos, angle, speed, proto);
  if (proto_collection[proto].animated)
    game::smanager -> get_sprite(sprite_num) -> start_animation(proto_collection[proto].frame_animation);
  else
    game::smanager -> get_sprite(sprite_num) -> set_frame(proto_collection[proto].frame_animation);

  game::smanager -> get_sprite(sprite_num) -> set_scale(proto_collection[proto].scale);
  game::smanager -> get_sprite(sprite_num) -> set_tint(proto_collection[proto].r,
						       proto_collection[proto].g,
						       proto_collection[proto].b);
  game::smanager -> get_sprite(sprite_num) -> set_alpha(proto_collection[proto].a);
  GLuint handle = this -> free_handle;
  collection.insert(std::pair<GLuint, CEnemyBullet*>(handle,bullet));
  while(collection.count(free_handle))
    ++free_handle;
  return handle;
}

GLuint CEnemyBulletManager::create_bullet_aimed(GLuint proto, GLfloat xpos, GLfloat ypos, 
						GLfloat speed, GLfloat xtarget, GLfloat ytarget, 
						GLfloat stray){
  GLfloat angle = 0.f;
  if ((xpos!=xtarget) || (ypos!=ytarget))
    angle = atan2(ytarget-ypos, xtarget-xpos)*180/M_PI+stray;
  return create_bullet(proto, xpos, ypos, speed, angle);
}

GLuint CEnemyBulletManager::create_bullet_aimed_hero(GLuint proto, GLfloat xpos, GLfloat ypos, 
						     GLfloat speed, GLfloat stray){
  return create_bullet_aimed(proto, xpos, ypos, speed, game::hero -> x, game::hero -> y, stray);
}

GLuint CEnemyBulletManager::destroy_bullet(GLuint handle){
  if (collection.count(handle) == 0)
    return 0;
  //если это интересно движку, сохраняем хэндл пули в сет уничтоженных
  if (collection[handle]->managed){
    destroyed_collection.insert(handle);
  }
  GLuint sprite_no = collection[handle] -> sprite_no;
  GLuint proto_no = collection[handle] -> proto_no;
  CSprite* sprite = game::smanager -> get_sprite(sprite_no);
  if (sprite != NULL){
    //Вызываем скрипт исчезновения пули с параметром номера спрайта
    game::script -> run_function(proto_collection[proto_no].die_func,sprite_no);
  }
  collection.erase(handle);
  if (free_handle>handle)
    free_handle=handle;
  return free_handle;
}

CEnemyBullet* CEnemyBulletManager::get_bullet(GLuint handle){
  if (collection.count(handle) == 1)
    return collection[handle];
  else
    return NULL;
}

void CEnemyBulletManager::think(){
  GLuint bad_handle;
  //  GLint graze;
  bullet_it i;
  for (i = collection.begin();i != collection.end();)
    switch (i -> second -> graze()){
    case BULLET_KILL:
      game::script -> run_function("hero_die");
    case BULLET_DESTROYED:
      bad_handle = i -> first;
      ++i;
      destroy_bullet(bad_handle);
      break;
    case BULLET_GRAZE:
      game::script -> run_function("hero_graze");
      //graze = game::script -> get_integer("graze");
      //game::script -> set_integer("graze",++graze);
      ++i;
      break;
    default:
      ++i;
    }
  
}

GLboolean CEnemyBulletManager::bullet_destroyed(GLuint handle){
  if (destroyed_collection.count(handle)>0){
    destroyed_collection.erase(handle);
    return true;
  }
  if (!collection[handle]->managed)
    collection[handle]->managed = true;
  return false;
}

GLuint CEnemyBulletManager::destroy_bullets_circle(GLfloat x, 
						   GLfloat y, 
						   GLfloat r){
  bullet_it i;
  GLuint bullet_counter = 0;
  for (i=collection.begin();i!=collection.end();){
    GLfloat bullet_x = game::smanager -> get_sprite(i->second->sprite_no) -> get_xpos();
    GLfloat bullet_y = game::smanager -> get_sprite(i->second->sprite_no) -> get_ypos();
    if (hypot(fabs(bullet_x - x),fabs(bullet_y - y)) <= r){
      GLuint bad_handle = i -> first;
      ++i;
      destroy_bullet(bad_handle);
      ++bullet_counter;
    }
    else
      ++i;
  }
  return bullet_counter;
}

GLuint CEnemyBulletManager::destroy_bullets_rectangle(GLfloat x1, GLfloat y1,
						      GLfloat x2, GLfloat y2){
  bullet_it i;
  GLuint bullet_counter = 0;
  for (i=collection.begin();i!=collection.end();){
    GLfloat bullet_x = game::smanager -> get_sprite(i->second->sprite_no) -> get_xpos();
    GLfloat bullet_y = game::smanager -> get_sprite(i->second->sprite_no) -> get_ypos();
    //TODO: нормальная проверка
    if ((bullet_x >= x1) && (bullet_x <= x2) &&
	(bullet_y >= y1) && (bullet_y <= y2)){
      GLuint bad_handle = i -> first;
      ++i;
      destroy_bullet(bad_handle);
      ++bullet_counter;
    }
    else
      ++i;
  }
  return bullet_counter;
}

GLuint CEnemyBulletManager::destroy_bullets_all(){
  bullet_it i;
  GLuint bullet_counter = 0;
  for (i=collection.begin();i!=collection.end();){
    GLuint bad_handle = i -> first;
    ++i;
    destroy_bullet(bad_handle);
    ++bullet_counter;
  }
  return bullet_counter;
}
