# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: MIT
#

## Dynamic
ADD_SUBDIRECTORY(books)
ADD_SUBDIRECTORY(stitches)
ADD_SUBDIRECTORY(statistics)
ADD_SUBDIRECTORY(reports)
ADD_SUBDIRECTORY(views)
ADD_SUBDIRECTORY(various)

## MusicBrainz DiscID
FIND_PACKAGE(DISCID)
IF (DISCID_FOUND)
 SET (CDTRACKING_SUPPORT ON)
 SET (CDTRACKING_LIBRARIES ${DISCID_LIBRARIES})
 MESSAGE (STATUS "MusicBrainz DiscID found.")
 ADD_SUBDIRECTORY(cdsvinyl)
ENDIF (DISCID_FOUND)
