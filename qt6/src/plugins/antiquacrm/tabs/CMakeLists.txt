# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

## Dynamic
ADD_SUBDIRECTORY(books)
ADD_SUBDIRECTORY(stitches)

## MusicBrainz DiscID
FIND_PACKAGE(DISCID)
IF (DISCID_FOUND)
 SET (CDTRACKING_SUPPORT ON)
 SET (CDTRACKING_LIBRARIES ${DISCID_LIBRARIES})
 MESSAGE (STATUS "MusicBrainz DiscID found.")
 ADD_SUBDIRECTORY(cdsvinyl)
ENDIF (DISCID_FOUND)
