-- @brief Provider Orders in Year
-- @file statistics_provider_volume_year.sql
SELECT o_provider_name AS pr_name, a_sell_price AS pr_price
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
WHERE (o_order_status=4 OR o_order_status=6)
  AND (o_payment_status=1 OR o_payment_status=4)
  AND date_part('year', o_since)=@SQL_WHERE_CLAUSE@
ORDER BY o_provider_name,a_sell_price DESC;
