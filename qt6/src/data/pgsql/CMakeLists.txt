# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8

SET (query_tables_FILES
  query_antiquacrm_cacheconf.sql
  query_antiquacrm_config.sql
  query_article_order_with_id.sql
  query_articles_exists.sql
  query_article_to_refunding.sql
  query_book_binding.sql
  query_book_publisher_de.sql
  query_build_refunding.sql
  query_charts_year_range.sql
  query_company_data.sql
  query_connection_info.sql
  query_customer_data.sql
  query_customer_exists.sql
  query_customer_from_order.sql
  query_customer_new_order.sql
  query_customer_orders_status.sql
  query_customer_overview.sql
  query_customer_recipient_address.sql
  query_default_delivery_service.sql
  query_designation_edit.sql
  query_designation.sql
  query_keywords.sql
  query_monthly_report.sql
  query_order_articles.sql
  query_order_by_oid.sql
  query_order_exists.sql
  query_order_import_customer.sql
  query_order_notifiaction.sql
  query_postal_codes.sql
  query_printing_billing_info.sql
  query_printing_cards.sql
  query_printing_delivery_note.sql
  query_printing_invoice.sql
  query_printing_refunding.sql
  query_printing_admonition.sql
  query_provider_order_exists.sql
  query_provider_orders.sql
  query_returning_articles.sql
  query_storage_by_id.sql
  query_storage_location.sql
  query_tab_books_main.sql
  query_tab_cdv_main.sql
  query_tab_config.sql
  query_tab_customers_main.sql
  query_tab_orders_main.sql
  query_tab_prints_main.sql
  query_tab_various_main.sql
  query_template_keys.sql
  query_tpl_mail_article.sql
  query_views.sql
  select_statement_postalcode_tables.sql
  select_statement_template_body.sql
  statistics_from_until_delivery_year.sql
  statistics_monthly_categories.sql
  statistics_payments_storage.sql
  statistics_payments_month.sql
  statistics_payments_year.sql
  statistics_provider_orders_year.sql
  statistics_provider_volume_year.sql
  union_default_delivery.sql
  union_printing_data.sql
  union_template_keys.sql
  union_template_values.sql
)

IF (CMAKE_HOST_WIN32)
 FILE (COPY ${query_tables_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ELSE (CMAKE_HOST_WIN32)
 INSTALL (FILES ${query_tables_FILES}
   DESTINATION share/${PROJECTNAME}/data/pgsql
   COMPONENT data
 )
ENDIF (CMAKE_HOST_WIN32)
