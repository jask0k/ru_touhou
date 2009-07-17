cmake_minimum_required(VERSION 2.6)
find_program(LUA_COMPILER 
  NAMES luac luac5.1)

set (LUA_B init.luc level1.luc)

add_custom_target (scripts ALL DEPENDS ${LUA_B})

foreach (COMPILED_SCRIPT ${LUA_B})
  string(REPLACE ".luc" ".lua" SCRIPTNAME ${COMPILED_SCRIPT})
  add_custom_command( 
    OUTPUT ${COMPILED_SCRIPT} 
    COMMAND ${LUA_COMPILER} -o ${COMPILED_SCRIPT} ${SCRIPTNAME} 
    MAIN_DEPENDENCY ${SCRIPTNAME})
endforeach(COMPILED_SCRIPT)

find_program(ZIP_PACKER
  NAMES pkzip zip)

set (DATA_FILE th_ru.dat)
set (PICTURES ui.png grnd03.jpg)
set (SPRITESHEETS aya.png aya_2.png bullets.png fontg.png)

foreach (SP_SHEET ${SPRITESHEETS})
	list (APPEND ANIMATIONS ${SP_SHEET}.spr)
endforeach()

add_custom_target (datafile ALL DEPENDS ../${DATA_FILE})

add_custom_command(
	OUTPUT ../${DATA_FILE}
	COMMAND ${ZIP_PACKER} ../${DATA_FILE} 
	${LUA_B} ${PICTURES}
	${SPRITESHEETS} ${ANIMATIONS}
	DEPENDS ${LUA_B} ${PICTURES} ${SPRITESHEETS} ${ANIMATIONS}
)
