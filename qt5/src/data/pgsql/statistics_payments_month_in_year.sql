-- @brief Statistics Payment of  the Year
-- @file statistics_payments_month_in_year.sql
SELECT DATE(o_delivered) as day, EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch, count(o_id) AS counts
FROM inventory_orders
WHERE date_part('year', o_delivered) = (@SQL_WHERE_CLAUSE@ - 1) GROUP BY day
UNION
SELECT DATE(o_delivered) as day, EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch, count(o_id) AS counts
FROM inventory_orders
WHERE date_part('year', o_delivered) = @SQL_WHERE_CLAUSE@
GROUP BY day ORDER BY sepoch;
