# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (query_tables_FILES
  query_article_order_with_id.sql
  query_articles_exists.sql
  query_book_binding.sql
  query_book_publisher_de.sql
  query_customer_exists.sql
  query_customer_orders_status.sql
  query_default_delivery_service.sql
  query_designation.sql
  query_keywords.sql
  query_order_articles.sql
  query_order_by_oid.sql
  query_order_exists.sql
  query_postal_codes.sql
  query_provider_order_exists.sql
  query_provider_order_history.sql
  query_storage_location.sql
  query_tab_books_main.sql
  query_tab_customers_main.sql
  query_tab_orders_main.sql
  query_views.sql
  query_printing_billing_info.sql
  query_template_body.sql
  query_template_keys.sql
  query_company_data.sql
)

SET (select_statement_FILES
 select_statement_postalcode_tables.sql
)

INSTALL (FILES ${query_tables_FILES} ${select_statement_FILES}
  DESTINATION share/${PROJECTNAME}/data/pgsql
  COMPONENT data
)
