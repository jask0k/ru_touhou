#ifndef _COPYPASTA_HPP
#define _COPYPASTA_HPP

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include <iostream>
#include <cmath>
#include "SDL_rwops_zzip.h"

#define TEXTURE_LOAD_FAIL 0
#define TEXTURE_LOAD_SUCCESS 1
GLuint LoadTexture(const char* filename, GLuint& texture, SDL_Surface*& surface);
GLuint LoadTexture_simple(const char* filename);
void glEnable2D();
void glDisable2D();
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );

#endif
