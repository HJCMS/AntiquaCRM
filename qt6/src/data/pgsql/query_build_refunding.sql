-- @brief Search Article Refund Payment Information
-- @file query_build_refunding.sql
SELECT a_payment_id, a_order_id, a_article_id, a_customer_id,
 a_count, a_title, a_price, CONCAT('-', a_sell_price)::NUMERIC(9,2) AS a_sell_price,
 a_modified, a_provider_id, a_type, a_refunds_cost, a_tax
FROM article_orders WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
