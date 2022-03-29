CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
## Project and Executable Name
SET (PROJECTNAME "antiquacrm")

PROJECT(${PROJECTNAME} VERSION 0.0.1 LANGUAGES CXX)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
    LinguistTools
)

#if(QT_VERSION_MAJOR EQUAL 6)
# TODO PRINT WARNING
#endif()

FIND_PACKAGE(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
  Core
  Widgets
  Sql
  Network
  LinguistTools
)

##############################################################
## SET My Targets
##############################################################

ADD_SUBDIRECTORY (icons)
ADD_SUBDIRECTORY (locales)
ADD_SUBDIRECTORY (src)

##EOF
