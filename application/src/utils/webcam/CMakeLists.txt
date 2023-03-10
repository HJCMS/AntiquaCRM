# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(_optionalProject "AntiquaWebCam")

SET (_optionalProject_FILES
 webcam.cpp
 webcam.h
)

ADD_LIBRARY (${_optionalProject} STATIC
 ${_optionalProject_FILES}
)

SET_TARGET_PROPERTIES (${_optionalProject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_optionalProject}
)

TARGET_LINK_LIBRARIES (${_optionalProject} PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Multimedia
  Qt${QT_VERSION_MAJOR}::MultimediaWidgets
  AntiquaCRM
)
