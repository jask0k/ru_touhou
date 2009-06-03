#include "back.hpp"
void CBack::init(const char* filename, GLint vy){
  this->flightY = 0;
  this->vy = vy;
  ground_texture = LoadTexture_simple(filename);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(ground_texture,GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void CBack::draw(){
  glPushMatrix();{
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)GAME_FIELD_WIDTH/(GLfloat)GAME_FIELD_HEIGHT, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    //    glTranslatef(0.0f,.0f,0.0f);
    glRotatef(-45.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, ground_texture);
    glBegin(GL_QUADS);{
      glTexCoord2i(0,flightY); glVertex3i(-1,-1,-1);
      glTexCoord2i(GAME_FIELD_WIDTH,flightY); glVertex3i(1,-1,-1);
      glTexCoord2i(GAME_FIELD_WIDTH,flightY+100); glVertex3i(1,1,-1);
      glTexCoord2i(0, flightY+100);glVertex3i(-1,1,-1);
    }
    glEnd();
  }
  glPopMatrix();
}

void CBack::think(){
  flightY++;
}
