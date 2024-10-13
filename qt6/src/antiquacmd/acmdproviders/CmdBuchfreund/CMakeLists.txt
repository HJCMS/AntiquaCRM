# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET (_pluginTarget "CmdBuchfreund")

INCLUDE_DIRECTORIES (
  ${ANTIQUACMD_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY (${_pluginTarget} SHARED
  cmdbuchfreund.json
  cmdbuchfreund.h cmdbuchfreund.cpp
)

SET_TARGET_PROPERTIES (${_pluginTarget} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_pluginTarget}
)

TARGET_LINK_LIBRARIES (${_pluginTarget} PRIVATE
  Qt6::Core
  Qt6::Sql
  Qt6::Network
  Qt6::Xml
  AntiquaCRM
  ACmdPlugin
)

INSTALL (TARGETS ${_pluginTarget}
  DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/antiquacmd/plugins
  COMPONENT Libraries
)
