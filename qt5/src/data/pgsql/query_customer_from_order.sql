-- @brief Query Customer with Current OrderId
-- @file query_customer_from_order.sql
SELECT o_customer_id FROM inventory_orders WHERE @SQL_WHERE_CLAUSE@;

