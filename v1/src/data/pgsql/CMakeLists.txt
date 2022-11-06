# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (query_tables_FILES
 query_book_binding.sql
 query_book_publisher_de.sql
 query_customer_orders_status.sql
 query_designation.sql
 query_keywords.sql
 query_order_by_oid.sql
 query_postal_codes.sql
 query_storage_location.sql
 query_tab_books_main.sql
 query_tab_customers_main.sql
 query_tab_orders_main.sql
 query_union_postal_codes.sql
 query_views.sql
)

SET (select_statement_FILES
 select_statement_postalcode_tables.sql
)

INSTALL (FILES ${query_tables_FILES} ${select_statement_FILES}
  DESTINATION share/${PROJECTNAME}/data/pgsql
  COMPONENT data
)
