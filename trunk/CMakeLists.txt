cmake_minimum_required (VERSION 2.6) 
project (danmaku)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(Lua51 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLU REQUIRED)

find_library(ZZIP_LIBRARY zzip)
find_path(ZZIP_INCLUDE_DIR zzip.h)

set (danmaku_INCLUDES 
  ${CMAKE_SOURCE_DIR} ${SDL_INCLUDE_DIR} 
  ${SDLMIXER_INCLUDE_DIR}
  ${SDLIMAGE_INCLUDE_DIR} ${LUA_INCLUDE_DIR} 
  ${OPENGL_INCLUDE_DIR} ${GLU_INCLUDE_DIR} 
  ${ZZIP_INCLUDE_DIR})

include_directories(${danmaku_INCLUDES})

if (${WIN32})
  add_definitions(-D_USE_MATH_DEFINES)
else ()
  set (CMAKE_CXX_FLAGS "-Wall -Wextra -ggdb3")
  add_definitions(-DDEBUG)
endif()

set (danmaku_LIBS 
  ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${LUA_LIBRARIES} 
  ${OPENGL_LIBRARIES} ${GLU_LIBRARIES} ${SDLMIXER_LIBRARY}
  ${ZZIP_LIBRARY} )
set (danmaku_SRCS 
  back.cpp controller.cpp copypasta.cpp game.cpp hero.cpp 
  main.cpp script.cpp sprite.cpp text.cpp music.cpp SDL_rwops_zzip.c)

add_executable (danmaku ${danmaku_SRCS})
target_link_libraries(danmaku ${danmaku_LIBS})

add_subdirectory(th_ru)
