-- @brief Query Customer AntiquaCRM::OrderStatus
-- @file query_customer_orders_status.sql
SELECT o_id AS orderid,
 a_article_id AS article,
 o_invoice_id AS invoice,
 o_provider_name AS provider,
 o_provider_order_id AS prorder,
 o_since AS since,
 o_delivered AS deliver
FROM customers LEFT JOIN inventory_orders ON o_customer_id=c_id
LEFT JOIN article_orders ON a_customer_id=c_id
WHERE @SQL_WHERE_CLAUSE@ AND o_order_status=5 AND o_payment_status=true ORDER BY o_id;
