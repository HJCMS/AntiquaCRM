-- @brief Query Articles for Returning Job
-- @file query_returning_articles.sql
SELECT a_payment_id, a_sell_price FROM article_orders
LEFT JOIN inventory_orders ON o_id=a_order_id
WHERE @SQL_WHERE_CLAUSE@
ORDER BY a_order_id,a_payment_id;
