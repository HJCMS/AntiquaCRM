# -*- coding: utf-8 -*-
# vim: SETfileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sub_application "antiquacmd")

SET(ANTIQUACMD_INCLUDE_DIRS
  ${ANTIQUACRM_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/acmdproviders
  ${CMAKE_CURRENT_BINARY_DIR}/acmdproviders
)

ADD_SUBDIRECTORY(acmdproviders)

INCLUDE_DIRECTORIES (${ANTIQUACMD_INCLUDE_DIRS})

ADD_EXECUTABLE(${_sub_application}
  main.cpp
  antiquacmd.h antiquacmd.cpp
)

TARGET_COMPILE_FEATURES(${_sub_application} PUBLIC cxx_std_17)

TARGET_LINK_LIBRARIES(${_sub_application} PRIVATE
  Qt6::Core
  Qt6::Network
  Qt6::Sql
  Qt6::Xml
  AntiquaCRM
  ACmdPlugin
)

SET_TARGET_PROPERTIES(${_sub_application} PROPERTIES
  OUTPUT_NAME ${_sub_application}
  LINKER_LANGUAGE CXX
)

INSTALL (TARGETS ${_sub_application}
  RUNTIME DESTINATION lib${LIB_SUFFIX}/antiquacmd/
  COMPONENT Core
)
