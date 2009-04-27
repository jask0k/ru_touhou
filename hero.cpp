#include "hero.hpp"
#include <iostream>
#include "SDL_image.h"

CHero::CHero(){
  x = 100;
  y = 100;
  vx = vy = speed = angle = 0;
  w = 32;
  h = 64;
}

CHero::CHero(char* texture_file){
  CHero();
  load_texture(IMG_Load(texture_file));
}

void CHero::draw(){
#ifdef DEBUG
  std::cerr << "Begin to draw Aya.";
#endif
  glBindTexture( GL_TEXTURE_2D, texture );
 
  glBegin( GL_QUADS );
        //Top-left vertex (corner)
        glTexCoord2i( 0, 0 );
        glVertex3f( 100, 100, 0.0f );
	
	//Bottom-left vertex (corner)
	glTexCoord2i( 2, 0 );
	glVertex3f( 228, 100, 0 );
	
	//Bottom-right vertex (corner)
	glTexCoord2i( 2, 2 );
	glVertex3f( 228, 228, 0 );
	
	//Top-right vertex (corner)
	glTexCoord2i( 0, 2 );
	glVertex3f( 100, 228, 0 );
  glEnd();
#ifdef DEBUG
  std::cerr << ".done!" << std::endl;
#endif
}

GLuint CHero::load_texture (SDL_Surface* texture_face){
  /*  GLenum texture_format;
  GLint  nOfColors;

#ifdef DEBUG
  if ( (texture_face->w & (texture_face->w - 1)) != 0 ) {
    std::cerr << "Warning: Width isn't power of two!" << std::endl;
  }
	
  if ( (texture_face->h & (texture_face->h - 1)) != 0 ) {
    std::cerr << "Warning: Height isn't power of two!" << std::endl;
  }
  std::cerr << "Transforming texture.";
#endif
  nOfColors = texture_face->format->BytesPerPixel;
  if (nOfColors == 4){
    if (texture_face->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA;
  }
  else if (nOfColors == 3){
    if (texture_face->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR;
  } 
  else {
    std::cerr << "warning: the image is not truecolor..  this will probably break" << std::endl;
    // this error should not go unhandled
  }
        
  // Have OpenGL generate a texture object handle for us
  glGenTextures( 1, &texture );
 
  // Bind the texture object
  glBindTexture( GL_TEXTURE_2D, texture );
  
  // Set the texture's stretching properties
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  
  // Edit the texture object's image data using the information SDL_Surface gives us
  glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, texture_face->w, texture_face->h, 0,
		texture_format, GL_UNSIGNED_BYTE, texture_face->pixels );
#ifdef DEBUG
  std::cerr << ".done!" <<std::endl;
#endif*/
  texture = 0;
  return texture;
}
