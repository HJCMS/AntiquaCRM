# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

IF (CMAKE_HOST_LINUX)
  ## Enable Development build
  SET (ANTIQUA_DEVELOPMENT BOOL ON)
  SET (WITH_ANTIQUACMD BOOL ON)
  SET (ANTIQUACRM_STD_STANDARD 20)
  SET (ZLIB_ROOT PATH /usr)
ENDIF (CMAKE_HOST_LINUX)

IF (CMAKE_HOST_WIN32)
  SET (USER_CUSTOM_INCLUDE_PATH CACHE PATH "Custom include path")
  IF (IS_DIRECTORY ${USER_CUSTOM_INCLUDE_PATH})
    MESSAGE (STATUS "Windows add CMAKE_CXX_FLAGS -I${USER_CUSTOM_INCLUDE_PATH}!")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${USER_CUSTOM_INCLUDE_PATH}")
  ENDIF ()
  SET (USER_CUSTOM_LIBRARY_PATH CACHE PATH "Custom libs path")
  SET (ZLIB_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (ZLIB_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
  SET (QRENCODE_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (QRENCODE_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
  SET (DISCID_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (DISCID_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
ENDIF ()
