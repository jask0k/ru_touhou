cmake_minimum_required(VERSION 2.8) 
project(danmaku)

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(Lua51 REQUIRED)
find_package(PhysFS REQUIRED)
find_package(OpenGL REQUIRED)

set(danmaku_INCLUDES 
  ${CMAKE_SOURCE_DIR}
  ${SDL_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR} ${PHYSFS_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR} 
)

include_directories(${danmaku_INCLUDES})

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -ggdb3 -DDEBUG")
  set(CMAKE_C_FLAGS_DEBUG "-Wall -Wextra -ggdb3 -DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "-s -O3")
  set(CMAKE_C_FLAGS_RELEASE "-s -O3")
endif()

if(MSVC)
  add_definitions(-D_USE_MATH_DEFINES)
  add_definitions(-D_GNU_SOURCE=1)
endif()

if(MINGW OR MSYS)
  if(CMAKE_C_COMPILER MATCHES ".*/(.*)gcc*")
    set(CMAKE_RC_COMPILER "${CMAKE_MATCH_1}windres")
  else()
    set(CMAKE_RC_COMPILER "windres")
  endif()
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <SOURCE> -O coff -o <OBJECT>")
  enable_language(RC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--subsystem,windows")
  set(SDL_LIBRARY mingw32;libSDLmain.a;libSDL.a;libwinmm.a;libgdi32.a;libkernel32.a;libmsvcrt.a;libuser32.a)
  set(SDLIMAGE_LIBRARY libSDL_image.a;libpng.a;libtiff.a;libjpeg.a;libz.a)
  set(SDLMIXER_LIBRARY libSDL_mixer.a;libSDL.a;libvorbisfile.a;libvorbis.a;libogg.a;libmikmod.a;libmad.a;libflac.a;libwinmm.a)
  set(PHYSFS_LIBRARY libphysfs.a)
  set(LUA_LIBRARY liblua.a)
endif()

set(danmaku_LIBS 
  ${SDL_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY}
  ${LUA_LIBRARY} ${PHYSFS_LIBRARY}
  ${OPENGL_LIBRARIES} tolua++
)

set(danmaku_SRCS 
  back.cpp controller.cpp copypasta.cpp game.cpp hero.cpp enemy_bullet.cpp variables.cpp
  main.cpp particles.cpp script.cpp sprite.cpp text.cpp music.cpp enemy.cpp physfsrwops.c
  danmaku.rc resolution.cpp danmaku_api.hpp hero_bullet.cpp
)

#хедеры, из которых генерится апи, не забываем прописывать //tolua_export, //tolua_begin, //tolua_end
set(danmaku_HEADERS
  back.hpp controller.hpp enemy.hpp enemy_bullet.hpp game.hpp hero.hpp music.hpp particles.hpp
  sprite.hpp text.hpp variables.hpp hero_bullet.hpp)



#генерим апи 
add_custom_command( 
  OUTPUT danmaku_api.hpp 
#у меня tolua++ с параметром -o сегфолтится почему-то
  COMMAND tolua++ danmaku_api.pkg > danmaku_api.hpp 
  MAIN_DEPENDENCY danmaku_api.pkg
  DEPENDS ${danmaku_HEADERS})


add_executable(danmaku ${danmaku_SRCS})
target_link_libraries(danmaku ${danmaku_LIBS})

add_subdirectory(th_ru)
