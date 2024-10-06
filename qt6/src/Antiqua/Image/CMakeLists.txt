# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

SET(_sharedLibrary "AntiquaImage")

INCLUDE_DIRECTORIES (
 ${INTERNAL_LIBRARY_INCLUDE_DIRS}
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
)

SET (_library_FILES
  Images.dox
  antiquaimagedialog.h.cmake
)

CONFIGURE_FILE (antiquaimagedialog.h.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/antiquaimagedialog.h
)

SET (_library_config_HEADERS
  antiquaimagedialog.h
)

SET (_library_config_TEMPLATES
 AntiquaImageDialog
)

SET (_library_SOURCES
  imagefilesource.cpp
  imageimportdialog.cpp
  imagerubberband.cpp
  imagetreepathview.cpp
  imagetreeview.cpp
  imageviewer.cpp
  imageviewtoolbar.cpp
)

SET (_library_HEADERS
  imagefilesource.h
  imageimportdialog.h
  imagerubberband.h
  imagetreepathview.h
  imagetreeview.h
  imageviewer.h
  imageviewtoolbar.h
)

ADD_LIBRARY (${_sharedLibrary} SHARED
 ${_library_config_HEADERS}
 ${_library_config_TEMPLATES}
 ${_library_SOURCES}
 ${_library_HEADERS}
 ${_library_FILES}
)

SET_TARGET_PROPERTIES (${_sharedLibrary} PROPERTIES
 VERSION ${ANTIQUACRM_VERSION_STRING}
 SOVERSION ${ANTIQUACRM_VERSION_MAJOR}
 OUTPUT_NAME ${_sharedLibrary}
 LINKER_LANGUAGE CXX
)

TARGET_LINK_LIBRARIES (${_sharedLibrary} PRIVATE
 Qt6::Core
 Qt6::Gui
 Qt6::Widgets
 Qt6::Network
 Qt6::Sql
 Qt6::Xml
 AntiquaCRM
 AntiquaWidgets
)

IF (CMAKE_HOST_LINUX)
  INSTALL (TARGETS ${_sharedLibrary}
   LIBRARY DESTINATION lib${LIB_SUFFIX}/
   COMPONENT Libraries
   NAMELINK_COMPONENT Development
  )

  INSTALL (FILES ${library_HEADERS} ${_stl_HEADERS}
   DESTINATION include/AntiquaCRM
   COMPONENT Development
  )
ENDIF()

##EOF
