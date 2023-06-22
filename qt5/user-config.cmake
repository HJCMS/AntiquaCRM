# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (USER_CUSTOM_INCLUDE_PATH "" CACHE PATH "Custom include path")

SET (USER_CUSTOM_LIBRARY_PATH "" CACHE PATH "Custom libs path")

IF (CMAKE_HOST_WIN32)
# Comment out this line if you made any changes
MESSAGE (STATUS "Windows user-config.cmake is used!")
SET (ZLIB_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
SET (ZLIB_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
SET (QRENCODE_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
SET (QRENCODE_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
SET (DISCID_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
SET (DISCID_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
ENDIF (CMAKE_HOST_WIN32)
