# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_sub_application "antiquacrm_assistant")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_SUBDIRECTORY(icons)
SET (subproject_RESOURCES
 assistant.qrc
)

QT5_ADD_RESOURCES (QRC_FILES ${subproject_RESOURCES})

SET (subproject_SOURCES
 main.cpp
 configpgsql.cpp
 configssl.cpp
 assistant.cpp
 infopage.cpp
 testconfig.cpp
)

SET (subproject_HEADERS
 configpgsql.h
 configssl.h
 assistant.h
 infopage.h
 testconfig.h
)

ADD_EXECUTABLE (${_sub_application}
  ${subproject_HEADERS}
  ${subproject_SOURCES}
  ${subproject_RESOURCES}
  ${QRC_FILES}
)

SET_TARGET_PROPERTIES (${_sub_application} PROPERTIES
  OUTPUT_NAME ${_sub_application}
  LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_sub_application} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCRM
)

INSTALL (TARGETS ${_sub_application}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
  COMPONENT Core
)
