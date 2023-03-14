# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_sub_application "antiquacmd")

SET (SUB_PROJECT_INCLUDE_DIRS
  ${ANTIQUACRM_CURRENT_SOURCE_DIR}/src/lib
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/abebooks
  ${CMAKE_CURRENT_BINARY_DIR}/abebooks
  ${CMAKE_CURRENT_SOURCE_DIR}/booklooker
  ${CMAKE_CURRENT_BINARY_DIR}/booklooker
  ${CMAKE_CURRENT_SOURCE_DIR}/buchfreund
  ${CMAKE_CURRENT_BINARY_DIR}/buchfreund
)

INCLUDE_DIRECTORIES (${SUB_PROJECT_INCLUDE_DIRS})

SET (PROJECT_HEADERS
  antiquacmd.h
  settings.h
  networkrequest.h
  networker.h
  provider.h
  sqlpsql.h
  customers.h
)

SET (PROJECT_SOURCES
  main.cpp
  settings.cpp
  antiquacmd.cpp
  networkrequest.cpp
  networker.cpp
  provider.cpp
  sqlpsql.cpp
  customers.cpp
)

ADD_SUBDIRECTORY(abebooks)
ADD_SUBDIRECTORY(booklooker)
ADD_SUBDIRECTORY(buchfreund)

ADD_EXECUTABLE(${_sub_application}
  ${PROJECT_HEADERS}
  ${PROJECT_SOURCES}
)

TARGET_LINK_LIBRARIES(${_sub_application} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  ${ZLIB_LIBRARIES}
  AntiquaCRM
  AntiquacmdAbeBooks
  AntiquacmdBooklooker
  AntiquacmdBuchfreund
)

SET_TARGET_PROPERTIES(${_sub_application} PROPERTIES
  OUTPUT_NAME ${_sub_application}
  LINKER_LANGUAGE CXX
)

INSTALL (TARGETS ${_sub_application}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
  COMPONENT Core
)

##EOF

