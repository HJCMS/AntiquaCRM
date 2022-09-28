# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (sql_FILES
 query_tab_books_main.sql
)

file(COPY ${sql_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

INSTALL (FILES ${sql_FILES}
  DESTINATION share/${PROJECTNAME}/xml
  COMPONENT data
)
