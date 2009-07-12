#include "back.hpp"
void CBack::init(const char* filename, GLfloat vy){
  this->flightY = 0.0f;
  this->flightX = 0.0f;
  this->vy = vy;
  this->vx = 0.0f;
  this->rx = -45.f;
  this->vrx = 0.f;
  this->ry = this->vry = 0.0f;
  this->rz = this->vrz = 0.0f;

  this->fog_colour[0] = 0.3867;
  this->fog_colour[1] = 0.3867;
  this->fog_colour[2] = 0.2578;
  this->fog_colour[3] = 1.0;

  this->fog_density = 0.10f;
  this->vfog=0.f;
  this->ground_texture = LoadTexture_simple(filename);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void CBack::draw(){
  
  glPushMatrix();{

    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)GAME_FIELD_WIDTH/(GLfloat)GAME_FIELD_HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_EXP2);
    glFogfv (GL_FOG_COLOR, fog_colour);
    glFogf (GL_FOG_DENSITY, fog_density);
    glTranslatef(0.0f,-3.0f,-8.0f);
    glRotatef(rx,1.0f,0.0f,0.0f);
    glRotatef(ry,0.0f,1.0f,0.0f);
    glRotatef(rz,0.0f,0.0f,1.0f);
    glBindTexture(GL_TEXTURE_2D, ground_texture);
    glBegin(GL_QUADS);{
      glTexCoord2f(flightX+0,flightY+0); glVertex3f(-15,0,-1);
      glTexCoord2f(flightX+5,flightY+0); glVertex3f(15,0,-1);
      glTexCoord2f(flightX+5,flightY+5); glVertex3f(15,20,-1);
      glTexCoord2f(flightX+0,flightY+5); glVertex3f(-15,20,-1);
    }
    glEnd();
    glDisable (GL_FOG);
  }
  glPopMatrix();
}

void CBack::think(){
  flightY+=vy;
  flightX+=vx;
  rx+=vrx;
  ry+=vry;
  rz+=vrz;
  fog_density+=vfog;
}

void CBack::set_rotation(GLfloat rx, GLfloat ry, GLfloat rz){
  this -> rx = rx;
  this -> ry = ry;
  this -> rz = rz;
}

void CBack::set_speed(GLfloat vy, GLfloat vx){
  this -> vy = vy;
  this -> vx = vx;
}

void CBack::set_rotation_speed(GLfloat vrx, GLfloat vry, GLfloat vrz){
  this -> vrx = vrx;
  this -> vry = vry;
  this -> vrz = vrz;
}

void CBack::set_fog_colour(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha){
  this -> fog_colour[0] = red;
  this -> fog_colour[1] = green;
  this -> fog_colour[2] = blue;
  this -> fog_colour[3] = alpha;
}

void CBack::set_fog_density(GLfloat density, GLfloat vfog){
  this -> fog_density = density;
  this -> vfog = vfog;
}
