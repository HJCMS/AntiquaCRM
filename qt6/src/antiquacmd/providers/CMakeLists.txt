# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#


SET (_static "ACmdProviders")

INCLUDE_DIRECTORIES (
  ${ANTIQUACMD_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (${_static} STATIC
  acmdproviders.h acmdproviders.cpp
  acmdcustomers.h acmdcustomers.cpp
)

SET_TARGET_PROPERTIES (${_static} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_static}
)

TARGET_LINK_LIBRARIES (${_static} LINK_PRIVATE
  Qt6::Core
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  AntiquaCRM
)


ADD_SUBDIRECTORY(Booklooker)
# ADD_SUBDIRECTORY(AbeBooks)
# ADD_SUBDIRECTORY(Buchfreund)

##EOF
