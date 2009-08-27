#include "particles.hpp"

CParticle::CParticle(GLfloat size, struct RGBAcolour colour,
		     GLfloat x, GLfloat y, GLfloat vx, GLfloat vy, GLuint time_left):
  size(size),colour(colour),vx(vx),vy(vy),x(x),y(y),time_left(time_left){}

int CParticle::think(){
  x+=vx;
  if ((x < -size)||(x > GAME_FIELD_WIDTH+size))
    return PARTICLE_DESTROYED;
  y+=vy;
  if ((y < -size)||(y > GAME_FIELD_HEIGHT+size))
    return PARTICLE_DESTROYED;
  if (--time_left == 0 )
    return PARTICLE_DESTROYED;
  return PARTICLE_GOOD;
}

void CParticle::draw(){
  glPointSize(size);
  glPushAttrib(GL_CURRENT_BIT);
  glBegin(GL_POINTS);{
    glColor4f(colour.r,colour.g,colour.b,colour.a);
    glVertex2f(x,y);
  }
  glEnd();
  glPopAttrib();
}

CParticleManager::CParticleManager(){
  current_colour.a = 1.f;
  current_colour.r = 1.f;
  current_colour.g = 1.f;
  current_colour.b = 1.f;
  srand(time(NULL));
}

void CParticleManager::draw(){
  std::vector<CParticle*>::iterator i;
  for (i = collection.begin(); i != collection.end(); ++i)
    (*i) -> draw();
}

void CParticleManager::think(){
  std::vector<CParticle*>::iterator i;
  for (i = collection.begin(); i != collection.end();)
    if ((*i)->think() == PARTICLE_DESTROYED){
      delete *i;
      i = collection.erase(i);
    }
    else
      ++i;
}

void CParticleManager::set_colour(struct RGBAcolour colour){
  current_colour = colour;
}

void CParticleManager::set_colour(GLfloat r, GLfloat g, GLfloat b, GLfloat a){
  current_colour.r = r;
  current_colour.g = g;
  current_colour.b = b;
  current_colour.a = a;
}

void CParticleManager::create_from(GLfloat posx, GLfloat posy, GLuint decay){
  GLfloat angle = (GLfloat)(rand() % 360)/180*M_PI;
  GLfloat length = rand() % 30 +30;
  GLfloat size = (GLfloat)(rand() % 30+1)/10 ;
  GLfloat frame_path = (length/(GLfloat)decay);
  GLfloat vx = cos(angle)*frame_path;
  GLfloat vy = sin(angle)*frame_path;
  CParticle* new_part = new CParticle(size, current_colour, posx, posy, vx, vy, decay);
  collection.push_back(new_part);
}

void CParticleManager::create_angle(GLfloat posx, GLfloat posy, GLuint decay, GLfloat angle){
  GLfloat rad_angle = angle/180*M_PI;
  GLfloat length = rand() % 30 +30;
  GLfloat size = (GLfloat)(rand() % 50+1)/10 ;
  GLfloat frame_path = (length/(GLfloat)decay);
  GLfloat vx = cos(rad_angle)*frame_path;
  GLfloat vy = sin(rad_angle)*frame_path;
  CParticle* new_part = new CParticle(size, current_colour, posx, posy, vx, vy, decay);
  collection.push_back(new_part);
}

void CParticleManager::create_to(GLfloat posx, GLfloat posy, GLuint decay){
  GLfloat angle = (GLfloat)(rand() % 360)/180*M_PI;
  GLfloat length = rand() % 30 +30;
  GLfloat size = (GLfloat)(rand() % 50+1)/10 ;
  GLfloat frame_path = (length/(GLfloat)decay);
  GLfloat x = posx+cos(angle)*length;
  GLfloat y = posy+sin(angle)*length;
  GLfloat vx = -cos(angle)*frame_path;
  GLfloat vy = -sin(angle)*frame_path;
  CParticle* new_part = new CParticle(size, current_colour, x, y, vx, vy, decay);
  collection.push_back(new_part);
}
