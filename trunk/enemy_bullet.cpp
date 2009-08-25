#include "enemy_bullet.hpp"
 
CEnemyBullet::CEnemyBullet(GLuint sprite_no, GLfloat posx, GLfloat posy, 
			   GLfloat angle, GLfloat speed):
  sprite_no(sprite_no), grazed(false), acceleration(0.0f){
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
  
  //  if (game::script -> get_integer("god_mode") == 0){
    
    if (length <= bullet_hitbox + 3){
      return BULLET_KILL;
    } 
    else{
      if ((length <= bullet_hitbox + hero_grazebox)&& !(this -> grazed)){
	this -> grazed = true;
	return BULLET_GRAZE;
      }
    }
    //  }
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

CEnemyBulletManager::CEnemyBulletManager(std::string spritesheet):
  spritesheet(spritesheet),free_handle(1){}

GLuint CEnemyBulletManager::create_bullet(GLint frame, GLfloat xpos, GLfloat ypos, 
				   GLfloat speed, GLfloat angle){
  GLuint sprite_num = game::smanager -> create_sprite(spritesheet, LAYER_ENEMY_BULLET);
  CEnemyBullet* bullet = new CEnemyBullet(sprite_num, xpos, ypos, angle, speed);
  game::smanager -> get_sprite(sprite_num) -> set_frame(frame);
  GLuint handle = this -> free_handle;
  collection.insert(std::pair<GLuint, CEnemyBullet*>(handle,bullet));
  while(collection.count(free_handle))
    ++free_handle;
  return handle;
}

GLuint CEnemyBulletManager::create_bullet_aimed(GLint frame, GLfloat xpos, GLfloat ypos, 
						GLfloat speed, GLfloat xtarget, GLfloat ytarget, 
						GLfloat stray){
  GLfloat angle = 0.f;
  if ((xpos!=xtarget) || (ypos!=ytarget))
    angle = atan2(ytarget-ypos, xtarget-xpos)*180/M_PI+stray;
  return create_bullet(frame, xpos, ypos, speed, angle);
}

GLuint CEnemyBulletManager::create_bullet_aimed_hero(GLint frame, GLfloat xpos, GLfloat ypos, 
						     GLfloat speed, GLfloat stray){
  return create_bullet_aimed(frame, xpos, ypos, speed, game::hero -> x, game::hero -> y, stray);
}

GLuint CEnemyBulletManager::destroy_bullet(GLuint handle){
  if (collection.count(handle) == 0)
    return 0;
   // ЗДЕСЬ ДОЛЖНА БЫТЬ АНИМАЦИЯ ИСЧЕЗНОВЕНИЯ ПУЛИ
  CSprite* sprite = game::smanager -> get_sprite(collection[handle] -> sprite_no);
  if (sprite != NULL){
    sprite -> set_speed(0,0);
    sprite -> set_scale_speed(.2f);
    sprite -> set_alpha_speed(-.1f);
    sprite -> set_decay(10);
  }
    //  game::smanager -> destroy_sprite(get_bullet(handle) -> sprite_no);
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
  std::map<GLuint,CEnemyBullet*>::iterator i;
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
