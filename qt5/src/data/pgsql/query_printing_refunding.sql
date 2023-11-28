-- @brief Search Order Payment Information
-- @file query_printing_refunding.sql
SELECT a_payment_id, a_article_id, a_customer_id, a_count, a_title,
 a_modified, a_provider_id, a_type, a_tax, a_price,
 a_sell_price,
 a_refunds_cost
FROM article_orders WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
