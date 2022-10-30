# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (json_data_FILES
 european_countries.json
 iso_countrycodes.json
)

INSTALL (FILES ${json_data_FILES}
  DESTINATION share/${PROJECTNAME}/data/json
  COMPONENT data
)
