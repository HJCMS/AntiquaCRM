-- @brief Search Payment Article with Provider Order Id
-- @file query_provider_order_exists.sql
SELECT pr_name,pr_order,pr_customer_id,pr_order_id,pr_order_data
FROM provider_orders WHERE @SQL_WHERE_CLAUSE@ AND pr_ignore=false ORDER BY pr_order;
