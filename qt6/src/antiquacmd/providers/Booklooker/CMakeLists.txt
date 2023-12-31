# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (_pluginTarget "Booklooker")

INCLUDE_DIRECTORIES (
  ${ANTIQUACMD_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_DEFINITIONS (-DANTIQUACRM_SHARED_LIBRARY)

ADD_LIBRARY (${_pluginTarget} SHARED
  booklooker.json
  booklooker.h booklooker.cpp
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
)

TARGET_LINK_LIBRARIES (${_pluginTarget} LINK_PRIVATE
  Qt6::Core
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  AntiquaCRM
  ACmdProviders
)
