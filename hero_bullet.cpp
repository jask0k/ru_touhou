#include "hero_bullet.hpp"
CHeroBullet::CHeroBullet(GLuint proto_no, GLfloat speed, GLfloat angle, GLfloat xshift, GLfloat yshift){
  CHeroBullet(proto_no, speed, angle, game::hero->sprite);
  sprite -> x += xshift;
  sprite -> y += yshift;
}
CHeroBullet::CHeroBullet(GLuint proto_no, GLfloat speed, GLfloat angle, CSprite* shift){
  this -> proto_no = proto_no;
  game::hbmanager -> add(this);
  sprite -> x = shift->x;
  sprite -> y = shift->y;
  sprite -> set_angle(speed, angle);
}
CHeroBullet::~CHeroBullet(){
  game::hbmanager -> destroy(this);
}
GLboolean CHeroBullet::think(){
  return true;
}
CHeroBulletManager::CHeroBulletManager(){
  spritesheet = NULL;
}
void CHeroBulletManager::load_spritesheet(CSpriteSheet* ssheet){
  spritesheet = ssheet;
}
GLfloat CHeroBulletManager::collide(GLfloat x, GLfloat y, GLfloat hitbox_radius){
  std::set<CHeroBullet*>::iterator i;
  GLfloat damage=0.0f;
  for (i = collection.begin();i != collection.end();){
    CSprite* bullet_sprite = (*i)->sprite;
    if (hypot(x-bullet_sprite->x, y-bullet_sprite->y) < hitbox_radius+bullet_sprite -> get_width()){
      damage += proto_collection[(*i)->proto_no].damage;
      destroy(*(i++));
    }
    else
      ++i;
  }
  return damage;
}
void CHeroBulletManager::add(CHeroBullet *new_member){
  if (spritesheet == NULL){
#ifdef DEBUG
    std::cerr << "ERROR: bullet spritesheet unint!" << std::endl;
#endif
    return;
  }
  new_member -> sprite = new CSprite(spritesheet, LAYER_HERO_BULLET);
  SBulletProto proto = proto_collection[new_member -> proto_no];
  if (proto.animated)
    new_member -> sprite -> start_animation((GLuint)proto.frame_animation);
  else
    new_member -> sprite -> set_frame(proto.frame_animation);
  collection.insert(new_member);
}
GLuint CHeroBulletManager::create_proto(std::string die_func, GLfloat damage, 
					GLfloat scale, GLboolean animated, GLint frame_animation, 
					GLfloat r, GLfloat g, GLfloat b, GLfloat a){
  SBulletProto new_proto = {die_func, damage, scale, animated, frame_animation, r, g, b, a};
  proto_collection.push_back(new_proto);
  return proto_collection.size() -1;
}
void CHeroBulletManager::destroy(CHeroBullet* bullet){
  if (proto_collection[bullet->proto_no].die_func=="")
    delete bullet->sprite;
  else
    game::script -> run_function(proto_collection[bullet->proto_no].die_func,(void*)(bullet->sprite),"CSprite");
  collection.erase(bullet);
}
void CHeroBulletManager::think(){
}

