# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (DOXYGEN_FILES
 tabs.dox
)

ADD_SUBDIRECTORY(books)
ADD_SUBDIRECTORY(customers)
ADD_SUBDIRECTORY(cdsvinyl)
ADD_SUBDIRECTORY(orders)
ADD_SUBDIRECTORY(providers)
ADD_SUBDIRECTORY(prints)
ADD_SUBDIRECTORY(views)
ADD_SUBDIRECTORY(reports)
ADD_SUBDIRECTORY(various)

IF (Qt${QT_VERSION_MAJOR}Charts_FOUND)
ADD_SUBDIRECTORY(statistics)
ENDIF (Qt${QT_VERSION_MAJOR}Charts_FOUND)

## EOF
