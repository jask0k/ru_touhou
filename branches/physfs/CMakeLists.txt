cmake_minimum_required(VERSION 2.6) 
project(danmaku)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(Lua51 REQUIRED)
find_package(PhysFS REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

set(danmaku_INCLUDES 
  ${CMAKE_SOURCE_DIR} ${SDL_INCLUDE_DIR} 
  ${SDLMIXER_INCLUDE_DIR}
  ${SDLIMAGE_INCLUDE_DIR} ${LUA_INCLUDE_DIR} 
  ${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_DIR} 
  ${ZZIP_INCLUDE_DIR})

include_directories(${danmaku_INCLUDES})

IF(MSVC)
  add_definitions(-D_USE_MATH_DEFINES)
  add_definitions(-D_GNU_SOURCE=1)
else()
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb3")
  add_definitions(-DDEBUG)
endif()

if(MINGW OR MSYS)
  SET(CMAKE_RC_COMPILER "windres")
  SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <SOURCE> -O coff -o <OBJECT>")
  enable_language(RC)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--subsystem,windows")
endif()

set(danmaku_LIBS 
  ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${LUA_LIBRARY} 
  ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${SDLMIXER_LIBRARY}
  ${PHYSFS_LIBRARY})

set(danmaku_SRCS 
  back.cpp controller.cpp copypasta.cpp game.cpp hero.cpp enemy_bullet.cpp variables.cpp
  main.cpp particles.cpp script.cpp sprite.cpp text.cpp music.cpp enemy.cpp physfsrwops.c
  danmaku.rc)
  
add_executable(danmaku ${danmaku_SRCS})
target_link_libraries(danmaku ${danmaku_LIBS})

add_subdirectory(th_ru)
