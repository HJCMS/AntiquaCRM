# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

ADD_SUBDIRECTORY(pgsql)
ADD_SUBDIRECTORY(json)
ADD_SUBDIRECTORY(fonts)

SET(DATA_FILES
  antiquacrm.qcss
)

INSTALL (FILES ${DATA_FILES}
  DESTINATION share/${PROJECTNAME}/data
  COMPONENT data
)

##EOF
