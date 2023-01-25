# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET(source_FILES
 antiquacrm_untranslated.ts
 antiquacrm_de.ts
)

SET(source_LRELEASE
 antiquacrm_de.ts
)

## NOTE This file isn't a part of the default Qt5 Installation.
## https://doc.qt.io/qt-5/internationalization.html
## git submodule add https://code.qt.io/cgit/qt/qttranslations.git/tree/?h=v5.15.7-lts-lgpl qt5-tr
## wget -O qtbase_de.ts https://code.qt.io/cgit/qt/qttranslations.git/plain/translations/qtbase_de.ts
SET(qtr_FILES
 qt5-tr/translations/qtbase_de.ts
)

EXECUTE_PROCESS (COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_BINS OUTPUT_VARIABLE QT_BINARY_TARGET)

FIND_PROGRAM (LRELEASE_EXECUTABLE "lrelease" "lrelease-qt5" PATHS ${QT_BINARY_TARGET})
SET(LRELEASE_OPTIONS -compress -nounfinished -removeidentical)

FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
EXECUTE_PROCESS (COMMAND ${LRELEASE_EXECUTABLE} ${LRELEASE_OPTIONS} ${source_LRELEASE} ${qtr_FILES} -qm ${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}_de.qm
 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
 COMMAND_ECHO STDOUT
)
MESSAGE (STATUS "${PROJECTNAME}_de.qm")

IF (CMAKE_HOST_UNIX)
 FILE(GLOB QM_FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECTNAME}_de.qm")
 INSTALL (FILES ${QM_FILES}
  DESTINATION ${ANTIQUACRM_TRANSLATION_TARGET}
  COMPONENT data
 )
 INSTALL (FILES
  ${PROJECTNAME}_untranslated.ts
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${PROJECTNAME}
  COMPONENT data
 )
ENDIF (CMAKE_HOST_UNIX)
