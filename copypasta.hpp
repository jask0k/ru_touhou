#ifndef _COPYPASTA_HPP
#define _COPYPASTA_HPP

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_image.h>
#include <iostream>
#include <cmath>

#include "physfsrwops.h"

#define TEXTURE_LOAD_FAIL 0
#define TEXTURE_LOAD_SUCCESS 1
GLuint LoadTexture(const char* filename, GLuint& texture, SDL_Surface*& surface);
GLuint LoadTexture_simple(const char* filename);
void glEnable2D();
void glDisable2D();
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );

#endif
