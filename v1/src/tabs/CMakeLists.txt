# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

ADD_SUBDIRECTORY(books)
ADD_SUBDIRECTORY(customers)
ADD_SUBDIRECTORY(orders)
ADD_SUBDIRECTORY(providers)
ADD_SUBDIRECTORY(prints)
ADD_SUBDIRECTORY(views)
ADD_SUBDIRECTORY(reports)

IF (Qt${QT_VERSION_MAJOR}Charts_FOUND)
ADD_SUBDIRECTORY(statistics)
ENDIF (Qt${QT_VERSION_MAJOR}Charts_FOUND)

## EOF
