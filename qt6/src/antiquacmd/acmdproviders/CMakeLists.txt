# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_static_lib "ACmdPlugin")

INCLUDE_DIRECTORIES (
  ${ANTIQUACMD_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (${_static_lib} STATIC
  acmdproviders.h acmdproviders.cpp
  acmdcustomers.h acmdcustomers.cpp
  acmdconfig.h
)

ADD_DEFINITIONS(-DACMD_PLUGIN_EXPORT)

SET_TARGET_PROPERTIES (${_static_lib} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_static_lib}
  POSITION_INDEPENDENT_CODE ON
)

TARGET_LINK_LIBRARIES (${_static_lib} PRIVATE
  Qt6::Core
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  AntiquaCRM
)

ADD_SUBDIRECTORY(CmdAbeBooks)
ADD_SUBDIRECTORY(CmdBooklooker)
ADD_SUBDIRECTORY(CmdBuchfreund)

##EOF
