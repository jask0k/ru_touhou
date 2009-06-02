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
    perspectiveGL(100.0f, GAME_FIELD_WIDTH/GAME_FIELD_HEIGHT, 0.0f, 100.0f);
    glLoadIdentity();
    glTranslatef(0.0f,100.0f,0.0f);
    //    glRotatef(-10.0f,1.0f,0.0f,0.0f);
    glBindTexture(GL_TEXTURE_RECTANGLE_ARB, ground_texture);
    glBegin(GL_QUADS);{
      glTexCoord2i(0,flightY); glVertex3i(-GAME_FIELD_WIDTH,-GAME_FIELD_HEIGHT,0);
      glTexCoord2i(GAME_FIELD_WIDTH,flightY); glVertex3i(GAME_FIELD_WIDTH,-GAME_FIELD_HEIGHT,0);
      glTexCoord2i(GAME_FIELD_WIDTH,flightY+100); glVertex3i(GAME_FIELD_WIDTH,100,0);
      glTexCoord2i(0, flightY+100);glVertex3i(-GAME_FIELD_WIDTH,100,0);
    }
    glEnd();
  }
  glPopMatrix();
}

void CBack::think(){
  flightY+=vy;
}
