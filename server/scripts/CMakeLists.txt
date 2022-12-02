# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (_FILES
  antiqua-booklooker-export
  antiqua-database-backup
  antiqua-images-upload
  antiqua-publisher-build
  antiqua-update-abebooks
  antiqua-whsoft-export
)

INSTALL (FILES ${_FILES}
  DESTINATION share/antiquacrm/scripts
  COMPONENT data
)
