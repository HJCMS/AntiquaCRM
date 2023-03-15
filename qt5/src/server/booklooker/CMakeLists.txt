# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

INCLUDE_DIRECTORIES (${SUB_PROJECT_INCLUDE_DIRS})

SET (_sub_target_FILES
 booklooker.cpp
 booklooker.h
)

ADD_LIBRARY (AntiquacmdBooklooker STATIC
  ${_sub_target_FILES}
)

SET_TARGET_PROPERTIES (AntiquacmdBooklooker PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME AntiquacmdBooklooker
)

TARGET_LINK_LIBRARIES (AntiquacmdBooklooker PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  AntiquaCRM
)
