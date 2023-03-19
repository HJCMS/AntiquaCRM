# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
#
# SPDX-License-Identifier: LGPL-3.0-or-later
#

SET (p_header_FILES
 abstractinput_p.h
)

INSTALL (FILES ${p_header_FILES}
  DESTINATION include/AntiquaCRM/private
  COMPONENT Development
)
