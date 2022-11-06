-- @brief Query PostgreSQL Views
-- @file query_order_by_oid.sql
SELECT o.*, c_postal_address,
  (SELECT func_shipping_address(o.o_customer_id)) AS c_shipping_address
FROM inventory_orders AS o
LEFT JOIN customers ON c_id=o.o_customer_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY o_id LIMIT 1;
