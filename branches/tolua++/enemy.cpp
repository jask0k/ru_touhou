#include "enemy.hpp"
#include "hero_bullet.hpp"

CEnemy::CEnemy(GLuint proto, GLuint bullet_proto, CSprite* sprite, GLfloat max_hp):
  proto_no(proto),bullet_proto(bullet_proto),hp(max_hp),sprite(sprite){}

CEnemy::~CEnemy(){
  game::enmanager -> destroy_enemy(this);
}

GLboolean CEnemy::think(){
  hp -= game::hbmanager -> collide(sprite->x,sprite->y,sprite->get_width()/2.f);
  return (hp > 0.f);
}

GLboolean CEnemy::damage(GLfloat amount){
  hp -= amount;
  return (hp > 0.f);
}

CEnemyBullet* CEnemy::shoot_at(GLfloat x, GLfloat y, GLfloat speed){
  return game::ebmanager->create_bullet_aimed(proto_no, get_x(), get_y(), x, y, speed, 0 );
}

CEnemyBullet* CEnemy::shoot_at_hero(GLfloat speed, GLfloat stray){
  return game::ebmanager->create_bullet_aimed_hero(proto_no, get_x(), get_y(), speed, stray);  
}

CEnemyBullet* CEnemy::shoot(GLfloat speed, GLfloat angle){
  return game::ebmanager->create_bullet(proto_no, get_x(), get_y(), speed, angle);  
}

GLfloat CEnemy::get_x(){
  return sprite -> x;
}

GLfloat CEnemy::get_y(){
  return sprite -> y;
}

void CEnemy::set_speed(GLfloat vx, GLfloat vy, GLfloat vr){
  sprite -> v_x = vx;
  sprite -> v_y = vy;
  sprite -> v_r = vr;
}

void CEnemy::start_animation(GLuint animation){
  sprite -> start_animation(animation);
}

CEnemyManager::CEnemyManager(){}

GLuint CEnemyManager::create_enemy_proto(CSpriteSheet* sheetname, GLuint animation, GLfloat max_hp, GLuint bullet_proto, std::string die_func){
  SEnemyProto new_proto = {sheetname, animation, max_hp, bullet_proto, die_func};
  proto_collection.push_back(new_proto);
  return proto_collection.size()-1;
}

CEnemy* CEnemyManager::create_enemy(GLuint proto, GLfloat x, GLfloat y){
  CSprite* sprite = new CSprite(proto_collection[proto].sheetname, LAYER_ENEMY);
  sprite -> set_position(x,y);
  CEnemy* new_enemy = new CEnemy(proto, proto_collection[proto].bullet_proto, 
				 sprite, proto_collection[proto].max_hp);
  collection.insert(new_enemy);
  return new_enemy;
}

void CEnemyManager::destroy_enemy(CEnemy* handle){
  if (collection.count(handle)){
    GLuint proto_no = handle -> proto_no;
    CSprite* sprite = handle -> sprite;
    if (sprite != NULL){
      if (proto_collection[proto_no].die_func!="")
	//Вызываем скрипт исчезновения врага с параметром спрайта
	game::script -> run_function(proto_collection[proto_no].die_func,(void*)sprite,"CSprite");
      else
	delete sprite;
    }
    collection.erase(handle);
  }
}

GLboolean CEnemyManager::enemy_destroyed(CEnemy* handle){
  return !collection.count(handle);
}

void CEnemyManager::think(){
  std::set<CEnemy*>::iterator i;
  for (i=collection.begin();i!=collection.end();){
    if ((*i) -> think())
      ++i;
    else
      delete *(i++);
  }
}

void CEnemyManager::damage_circle(GLfloat x, GLfloat y, GLfloat r, GLfloat amount){
  std::set<CEnemy*>::iterator i;
  for (i=collection.begin();i!=collection.end();i++){
    GLfloat bullet_x = (*i)->sprite -> x;
    GLfloat bullet_y = (*i)->sprite -> y;
    if (hypot(fabs(bullet_x - x),fabs(bullet_y - y)) <= r+(*i) -> sprite -> get_width())
      (*i)->damage(amount);
  }
}

void CEnemyManager::damage_ray(GLfloat x, GLfloat y, GLfloat angle, GLfloat amount, GLboolean stop){

  std::set<CEnemy*>::iterator i,min=collection.end();
  GLfloat min_dist = GAME_FIELD_HEIGHT*2;
  GLfloat k = tan(angle);
  GLfloat hypok = hypot(k,1);
  GLfloat kxy = k*x-y;
  for (i=collection.begin();i!=collection.end();i++){
    GLfloat enemy_x = (*i)->sprite -> x;
    GLfloat enemy_y = (*i)->sprite -> y;
    GLboolean damaged = false;
    if (hypot(x-enemy_x,y-enemy_y) <= (*i) -> sprite -> get_width())
      damaged=true;
    else if((-k*enemy_x+enemy_y+kxy)/hypok <= (*i) -> sprite -> get_width() && 
	    fabs(atan2(enemy_y - y, enemy_x - x) - angle) < M_PI/2)
      damaged=true;
    if (damaged){
      if (stop){
	if (hypot(enemy_x-x,enemy_y-y)<min_dist){
	  min = i;
	  min_dist = hypot(enemy_x-x,enemy_y-y);
	}
      }
      else
	(*i)->damage(amount);
    }
  }
  if (stop && min!=collection.end())
    (*min) ->damage(amount);
}
void CEnemyManager::damage_rectangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat amount){
  std::set<CEnemy*>::iterator i;
  for (i=collection.begin();i!=collection.end();++i){
    GLfloat enemy_x = (*i)->sprite -> x;
    GLfloat enemy_y = (*i)->sprite -> y;
    GLfloat enemy_width = (*i)->sprite -> get_width();
    GLfloat enemy_height = (*i)->sprite -> get_height();
    if ((enemy_x >= x1-enemy_width/2) && (enemy_x <= x2+enemy_width/2) &&
	(enemy_y >= y1-enemy_height/2) && (enemy_y <= y2+enemy_height/2)){
      (*i)->damage(amount);
    }
  }

}
void CEnemyManager::damage_all(GLfloat amount){
  std::set<CEnemy*>::iterator i;
  for (i=collection.begin();i!=collection.end();i++)
    (*i)->damage(amount);
}
