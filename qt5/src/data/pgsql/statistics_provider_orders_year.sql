-- @brief Provider Orders in Year
-- @file statistics_provider_orders_in_year.sql
SELECT o_provider_name AS pr_name, count(o_provider_name) AS pr_count FROM inventory_orders
WHERE o_order_status=4 AND o_payment_status=1 AND date_part('year', o_since)=@SQL_WHERE_CLAUSE@
GROUP BY o_provider_name;
