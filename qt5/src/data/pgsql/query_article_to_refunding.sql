-- @brief Search Article to select for Refund.
-- @file query_article_to_refunding.sql
SELECT a.*
FROM inventory_orders
LEFT JOIN article_orders AS a ON a.a_order_id=o_id
WHERE @SQL_WHERE_CLAUSE@
  AND o_order_status=4 AND o_payment_status>0
ORDER BY o_id;
