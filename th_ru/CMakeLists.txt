cmake_minimum_required(VERSION 2.8)
find_program(LUA_COMPILER NAMES luac luac5.1)

set(LUA_B functions.luc init.luc level1.luc hero.luc)

add_custom_target(scripts ALL DEPENDS ${LUA_B})

foreach(COMPILED_SCRIPT ${LUA_B})
  string(REPLACE ".luc" ".lua" SCRIPTNAME ${COMPILED_SCRIPT})
  if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  add_custom_command( 
    OUTPUT ${COMPILED_SCRIPT} 
    COMMAND ${LUA_COMPILER} -o ${COMPILED_SCRIPT} ${CMAKE_HOME_DIRECTORY}/th_ru/${SCRIPTNAME} 
    MAIN_DEPENDENCY ${SCRIPTNAME})
  else()
  add_custom_command( 
    OUTPUT ${COMPILED_SCRIPT} 
    COMMAND ${LUA_COMPILER} -s -o ${COMPILED_SCRIPT} ${CMAKE_HOME_DIRECTORY}/th_ru/${SCRIPTNAME} 
    MAIN_DEPENDENCY ${SCRIPTNAME})
  endif()
endforeach()

if (NOT (DEFINED COMPRESSION))
	set (COMPRESSION "zip")
endif (NOT (DEFINED COMPRESSION))

if (COMPRESSION STREQUAL "zip")
	find_program(PACKER NAMES pkzip zip)
	#-u - не добавлять файлы, которые уже есть
	#-j - не добавлять пути
	#-9 - максимальная компрессия
	set(PACKER_OPTS -9 -u -j)
elseif (COMPRESSION STREQUAL "7zip")
	find_program(PACKER NAMES 7z)
	set(PACKER_OPTS a)
endif (COMPRESSION STREQUAL "zip")

set(DATA_FILE th_ru.dat)
set(PICTURES ui.png grass.png icon.png ui_wide.png)
set(SPRITESHEETS aya.png aya_2.png fontg.png effects.png panel_labels.png
    level1.png lantern.png bullete.png fontscore.png mokou.png)
set(SOUNDS spawn.wav)
set(MUSIC music.xm)

foreach(SP_SHEET ${SPRITESHEETS})
  list(APPEND ANIMATIONS ${SP_SHEET}.spr)
endforeach()

foreach(RESOURCE ${PICTURES} ${SPRITESHEETS} ${ANIMATIONS} ${SOUNDS} ${MUSIC})
  list(APPEND RESOURCES ${CMAKE_HOME_DIRECTORY}/th_ru/${RESOURCE})
endforeach()
list(APPEND RESOURCES ${LUA_B})

add_custom_target(datafile ALL DEPENDS ${CMAKE_BINARY_DIR}/${DATA_FILE})

add_custom_command(
	OUTPUT ${CMAKE_BINARY_DIR}/${DATA_FILE}
	COMMAND ${PACKER} ${PACKER_OPTS} ${CMAKE_BINARY_DIR}/${DATA_FILE} ${RESOURCES}
	DEPENDS ${LUA_B} ${PICTURES} ${SPRITESHEETS} ${ANIMATIONS} ${SOUNDS} ${MUSIC}
)
