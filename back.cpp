#include "back.hpp"
void CBack::init(const char* filename, GLfloat vy){
  this->flightY = 0;
  this->vy = vy;
  ground_texture = LoadTexture_simple(filename);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void CBack::draw(){
  GLfloat density = 0.09;
  GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
  glPushMatrix();{

    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)GAME_FIELD_WIDTH/(GLfloat)GAME_FIELD_HEIGHT, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glEnable (GL_FOG);
    glFogi (GL_FOG_MODE, GL_EXP2);
    glFogfv (GL_FOG_COLOR, fogColor);
    glFogf (GL_FOG_DENSITY, density);
    glHint (GL_FOG_HINT, GL_NICEST);
    glTranslatef(0.0f,-4.0f,-8.0f);
    glRotatef(-45.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_2D, ground_texture);
    glBegin(GL_QUADS);{
      glTexCoord2f(0,flightY+0); glVertex3f(-10,0,-1);
      glTexCoord2f(5,flightY+0); glVertex3f(10,0,-1);
      glTexCoord2f(5,flightY+5); glVertex3f(10,20,-1);
      glTexCoord2f(0,flightY+5); glVertex3f(-10,20,-1);
    }
    glEnd();
  }
    glDisable (GL_FOG);
  glPopMatrix();
}

void CBack::think(){
  flightY+=vy;
}
