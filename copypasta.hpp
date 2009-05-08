#ifndef _COPYPASTA_HPP
#define _COPYPASTA_HPP

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include <iostream>

#define TEXTURE_LOAD_FAIL 0
#define TEXTURE_LOAD_SUCCESS 1
GLuint LoadTexture(const char* filename, GLuint* texture, SDL_Surface* surface);
void glEnable2D();
void glDisable2D();
#endif
