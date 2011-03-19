#include "enemy_bullet.hpp"
 
CEnemyBullet::CEnemyBullet(CSprite* sprite, GLfloat posx, GLfloat posy, 
			   GLfloat angle, GLfloat speed, GLuint proto):
  sprite(sprite), grazed(false), acceleration(0.0f), proto_no(proto){
  sprite -> x = posx;
  sprite -> y = posy;
  sprite -> set_angle(speed,angle);
}

CEnemyBullet::~CEnemyBullet(){
  game::ebmanager -> destroy_bullet(this);
}

int CEnemyBullet::graze(){
  GLfloat hero_x = game::hero -> x;
  GLfloat hero_y = game::hero -> y;
  if (sprite == NULL || game::smanager -> sprite_destroyed(sprite)){
    return BULLET_DESTROYED;
  }
  GLfloat bullet_x = sprite -> x;
  GLfloat bullet_y = sprite -> y;
  GLfloat bullet_hitbox = (sprite -> get_width()) * .3f;
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
  GLfloat bullet_x = sprite -> x;
  GLfloat bullet_y = sprite -> y;
  if ((posx != bullet_x)||(posy != bullet_y)){//смотрим, отличаются ли координаты пули и игрока
    GLfloat angle = atan2(posy - bullet_y, posx - bullet_x)*180/M_PI+stray;
    sprite -> set_angle(speed,angle);
    return 0;
  }
  return 1;
}

int CEnemyBullet::lock_on_hero(GLfloat stray, GLfloat speed){
  return this -> lock_on(game::hero -> x, game::hero -> y, stray, speed);
}

int CEnemyBullet::stop(){
  if (sprite == NULL || game::smanager -> sprite_destroyed(sprite))
    return 1;
  sprite -> v_x = 0.f;
  sprite -> v_y = 0.f;
  sprite -> v_r = 0.f;
  return 0;
}

int CEnemyBullet::stray(GLfloat angle){
  if (sprite == NULL || game::smanager -> sprite_destroyed(sprite))
    return 1;
  GLfloat direction = sprite -> get_direction();
  GLfloat speed = sprite -> get_speed();
  sprite -> set_angle(speed,direction+angle);
  return 0;
}

CEnemyBulletManager::CEnemyBulletManager(){}

GLuint CEnemyBulletManager::create_proto(CSpriteSheet* spritesheet, GLint frame_animation, GLboolean animated, GLfloat scale, std::string& die_func){
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

CEnemyBullet* CEnemyBulletManager::create_bullet(GLuint proto, GLfloat xpos, GLfloat ypos, 
					  GLfloat speed, GLfloat angle){
  if (proto>=proto_collection.size()){
    throw "incorrect proto";
  }
  CSprite* sprite = new CSprite(proto_collection[proto].spritesheet, LAYER_ENEMY_BULLET);
  CEnemyBullet* bullet = new CEnemyBullet(sprite, xpos, ypos, angle, speed, proto);
  if (proto_collection[proto].animated)
    sprite -> start_animation(proto_collection[proto].frame_animation);
  else
    sprite -> set_frame(proto_collection[proto].frame_animation);

  sprite -> scale = proto_collection[proto].scale;
  sprite -> set_tint(proto_collection[proto].r,
		     proto_collection[proto].g,
		     proto_collection[proto].b);
  sprite -> alpha = proto_collection[proto].a;
  //  GLuint handle = this -> free_handle;
  collection.insert(bullet);
  //  while(collection.count(free_handle))
  //    ++free_handle;
  return bullet;
}

CEnemyBullet* CEnemyBulletManager::create_bullet_aimed(GLuint proto, GLfloat xpos, GLfloat ypos, 
						GLfloat speed, GLfloat xtarget, GLfloat ytarget, 
						GLfloat stray){
  GLfloat angle = 0.f;
  if ((xpos!=xtarget) || (ypos!=ytarget))
    angle = atan2(ytarget-ypos, xtarget-xpos)*180/M_PI+stray;
  return create_bullet(proto, xpos, ypos, speed, angle);
}

CEnemyBullet* CEnemyBulletManager::create_bullet_aimed_hero(GLuint proto, GLfloat xpos, GLfloat ypos, 
						     GLfloat speed, GLfloat stray){
  return create_bullet_aimed(proto, xpos, ypos, speed, game::hero -> x, game::hero -> y, stray);
}

void CEnemyBulletManager::destroy_bullet(CEnemyBullet* handle){
  if (collection.count(handle) == 0)
    return;
  CSprite* sprite = handle -> sprite;
  GLuint proto_no = handle -> proto_no;
  if (sprite != NULL && !game::smanager->sprite_destroyed(sprite)){
    //Вызываем скрипт исчезновения пули с параметром указателя спрайта
    game::script -> run_function(proto_collection[proto_no].die_func,(void*)sprite, "CSprite");
  }
  collection.erase(handle);
  return;
}


void CEnemyBulletManager::think(){
  //  GLuint bad_handle;
  //  GLint graze;
  bullet_it i;
  for (i = collection.begin();i != collection.end();)
    switch ((*i) -> graze()){
    case BULLET_KILL:
      game::script -> run_function("hero_die");
      //заметте, здесь нет break, потому что пуля, убившая героя, уничтожается
    case BULLET_DESTROYED:
      delete *(i++);
      //      bad_handle = *i;
      //      ++i;
      //      delete bad_handle;
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

GLboolean CEnemyBulletManager::bullet_destroyed(CEnemyBullet* handle){
  return collection.count(handle);
}

GLuint CEnemyBulletManager::destroy_bullets_circle(GLfloat x, 
						   GLfloat y, 
						   GLfloat r){
  bullet_it i;
  GLuint bullet_counter = 0;
  for (i=collection.begin();i!=collection.end();){
    GLfloat bullet_x = (*i)->sprite -> x;
    GLfloat bullet_y = (*i)->sprite -> y;
    if (hypot(fabs(bullet_x - x),fabs(bullet_y - y)) <= r){
      delete *(i++);
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
    GLfloat bullet_x = (*i)->sprite -> x;
    GLfloat bullet_y = (*i)->sprite -> y;
    //TODO: нормальная проверка
    if ((bullet_x >= x1) && (bullet_x <= x2) &&
	(bullet_y >= y1) && (bullet_y <= y2)){
      delete *(i++);
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
    delete *(i++);
    // GLuint bad_handle = i -> first;
    // ++i;
    // destroy_bullet(bad_handle);
    ++bullet_counter;
  }
  return bullet_counter;
}
