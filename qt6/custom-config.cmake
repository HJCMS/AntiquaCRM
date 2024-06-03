# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

CMAKE_HOST_SYSTEM_INFORMATION(RESULT DEVELOPMENT_HOST QUERY FQDN)
IF (${DEVELOPMENT_HOST} MATCHES "\.hjcms$")
  MESSAGE (STATUS "Antiqua Development enabled for ${DEVELOPMENT_HOST}.")
  SET (ANTIQUA_DEVELOPMENT BOOL ON)
  SET (WITH_ANTIQUACMD BOOL ON)
  SET (ANTIQUACRM_STD_STANDARD 20)
  SET (ZLIB_ROOT /usr CACHE PATH "GNU:zlib install directory")
ENDIF ()

IF (CMAKE_HOST_WIN32)
  SET (USER_INSTALL_PREFIX CACHE PATH "Dependency Location for installed sources")
  SET (USER_CUSTOM_INCLUDE_PATH
    ${USER_INSTALL_PREFIX}/include
    CACHE PATH
    "Custom include path"
  )
  IF (IS_DIRECTORY ${USER_CUSTOM_INCLUDE_PATH})
    MESSAGE (STATUS "Windows add CMAKE_CXX_FLAGS -I${USER_CUSTOM_INCLUDE_PATH}!")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${USER_CUSTOM_INCLUDE_PATH}")
  ENDIF ()
  SET (USER_CUSTOM_LIBRARY_PATH
    ${USER_INSTALL_PREFIX}/lib
    CACHE PATH "Custom library path"
  )
  SET (ZLIB_ROOT ${USER_INSTALL_PREFIX} CACHE PATH "GNU:zlib install directory")
  SET (ZLIB_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (ZLIB_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
  SET (QRENCODE_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (QRENCODE_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
  SET (DISCID_INCLUDE_DIRS "${USER_CUSTOM_INCLUDE_PATH}")
  SET (DISCID_LIBRARIES "${USER_CUSTOM_LIBRARY_PATH}")
ENDIF ()
