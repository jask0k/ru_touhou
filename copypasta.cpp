#include "copypasta.hpp"
//copypasta from http://gpwiki.org/index.php/C:SDL_OGL
GLuint LoadTexture(char* filename, GLuint* texture, SDL_Surface* surface){
  GLenum texture_format;
  GLint  nOfColors;
  
  if ( (surface = IMG_Load(filename)) ) { 
 
    // Check that the image's width is a power of 2
    if ( (surface->w & (surface->w - 1)) != 0 ) {
      std::cerr << "warning: image width is not a power of 2" << std::endl;
    }
	
    // Also check if the height is a power of 2
    if ( (surface->h & (surface->h - 1)) != 0 ) {
      std::cerr << "warning: image height is not a power of 2" << std::endl;
    }
 
    // get the number of channels in the SDL surface
    nOfColors = surface->format->BytesPerPixel;
    if (nOfColors == 4)     // contains an alpha channel
      {
	if (surface->format->Rmask == 0x000000ff)
	  texture_format = GL_RGBA;
	else
	  texture_format = GL_BGRA;
      } else if (nOfColors == 3)     // no alpha channel
      {
	if (surface->format->Rmask == 0x000000ff)
	  texture_format = GL_RGB;
	else
	  texture_format = GL_BGR;
      } else {
      std::cerr << "warning: the image is not truecolor..  this will probably break" << std::endl;
      texture_format = GL_RGB4
      //TODO: this error should not go unhandled
    }
        
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, texture );
 
    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, *texture);
 
    // Set the texture's stretching properties
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
 
    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
		  texture_format, GL_UNSIGNED_BYTE, surface->pixels );
  } 
  else {
    std::cerr << "SDL could not load " << filename <<": " << SDL_GetError() << std:endl;
    SDL_Quit();
    return TEXTURE_LOAD_FAIL;
  }    
 
  return TEXTURE_LOAD_SUCCESS;
}
