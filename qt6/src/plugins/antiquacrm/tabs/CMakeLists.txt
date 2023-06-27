# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

ADD_SUBDIRECTORY(books)
ADD_SUBDIRECTORY(stitches)
ADD_SUBDIRECTORY(customers)
#ADD_SUBDIRECTORY(orders)
#ADD_SUBDIRECTORY(providers)
#ADD_SUBDIRECTORY(views)
#ADD_SUBDIRECTORY(reports)
#ADD_SUBDIRECTORY(various)

IF (CDTRACKING_SUPPORT)
 ADD_SUBDIRECTORY(cdsvinyl)
ENDIF (CDTRACKING_SUPPORT)
