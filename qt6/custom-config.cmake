# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

CMAKE_HOST_SYSTEM_INFORMATION(RESULT DEVELOPMENT_HOST QUERY FQDN)

IF (CMAKE_HOST_LINUX)
  IF (${DEVELOPMENT_HOST} MATCHES "\.hjcms$")
    MESSAGE (STATUS "Antiqua Development enabled for ${DEVELOPMENT_HOST}.")
    SET (ANTIQUA_DEVELOPMENT BOOL ON)
    SET (WITH_ANTIQUACMD BOOL ON)
    SET (ANTIQUACRM_STD_STANDARD 20)
    SET (ZLIB_ROOT /usr CACHE PATH "GNU:zlib install directory")
  ENDIF ()
ENDIF ()

IF (CMAKE_HOST_WIN32)
  ## Not supported on Windows systems.
  SET (WITH_ANTIQUACMD BOOL OFF)

  ## This must configured in Qt:Kit configuration!
  IF (NOT USER_INSTALL_PREFIX)
    MESSAGE (WARNING "The USER_INSTALL_PREFIX is not set to find GNU Utils!")
  ENDIF ()

  IF (IS_DIRECTORY ${USER_INSTALL_PREFIX})
    SET (ZLIB_ROOT ${USER_INSTALL_PREFIX} CACHE PATH "ZLIB install directory")
    SET (QRENCODE_ROOT ${USER_INSTALL_PREFIX} CACHE PATH "QRENCODE install directory")
    SET (DISCID_ROOT ${USER_INSTALL_PREFIX} CACHE PATH "DISCID install directory")

    SET (USER_CUSTOM_INCLUDE_PATH
      ${USER_INSTALL_PREFIX}/include
      CACHE PATH "Custom library path"
    )
    MARK_AS_ADVANCED (USER_CUSTOM_INCLUDE_PATH)

    IF (IS_DIRECTORY ${USER_CUSTOM_INCLUDE_PATH})
      MESSAGE (STATUS "Windows add CMAKE_CXX_FLAGS -I${USER_CUSTOM_INCLUDE_PATH}!")
      SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${USER_CUSTOM_INCLUDE_PATH}")
    ENDIF ()

    IF (IS_DIRECTORY ${USER_INSTALL_PREFIX}/lib)
      SET (USER_CUSTOM_LIBRARY_PATH
        ${USER_INSTALL_PREFIX}/lib
        CACHE PATH "Custom library path"
      )
    ELSEIF (IS_DIRECTORY ${USER_INSTALL_PREFIX}/lib64)
      SET (USER_CUSTOM_LIBRARY_PATH
        ${USER_INSTALL_PREFIX}/lib64
        CACHE PATH "Custom library path"
      )
    ENDIF ()
  ENDIF ()
ENDIF ()
