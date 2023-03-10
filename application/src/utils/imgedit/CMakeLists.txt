# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_subproject "AntiquaImgEdit")

INCLUDE_DIRECTORIES (
  ${PROJECT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

SET (subproject_LINKER
 AntiquaCRM
)

IF(WEBCAM_BUILD_SUPPORTED)
SET (subproject_LINKER
 ${subproject_LINKER}
 AntiquaWebCam
)
ENDIF()

SET (subproject_SOURCES
  imagedialog.cpp
  imageview.cpp
  imageactionsbar.cpp
  imageselecter.cpp
  imagelistview.cpp
  rubberband.cpp
  sourceinfo.cpp
  imagetoolbar.cpp
)

SET (subproject_HEADERS
  imagedialog.h
  imageview.h
  imageactionsbar.h
  imageselecter.h
  imagelistview.h
  rubberband.h
  sourceinfo.h
  imagetoolbar.h
  AntiquaImgEdit
)

ADD_LIBRARY (${_subproject} STATIC
  ${subproject_HEADERS}
  ${subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
)

TARGET_LINK_LIBRARIES (${_subproject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Gui
  Qt${QT_VERSION_MAJOR}::Widgets
  ${subproject_LINKER}
)
