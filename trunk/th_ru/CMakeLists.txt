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

