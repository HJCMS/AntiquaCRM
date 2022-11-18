-- @brief Search Payment Article with Provider Order Id
-- @file query_provider_order_exists.sql
SELECT pr_name,pr_order,pr_order_data FROM provider_order_history WHERE @SQL_WHERE_CLAUSE@ ORDER BY pr_order;
