# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (query_tables_FILES
 query_tab_books_main.sql
 query_postal_codes.sql
)

SET (select_statement_FILES
 select_statement_postalcode_tables.sql
)

INSTALL (FILES ${query_tables_FILES} ${select_statement_FILES}
  DESTINATION share/${PROJECTNAME}/data/pgsql
  COMPONENT data
)
