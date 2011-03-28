# this one is important
SET(CMAKE_SYSTEM_NAME Windows-GNU)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER i686-pc-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-pc-mingw32-g++)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH /usr/i686-pc-mingw32/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(MY_OWN_INSTALL_PREFIX "/usr/i686-pc-mingw32/usr/" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

