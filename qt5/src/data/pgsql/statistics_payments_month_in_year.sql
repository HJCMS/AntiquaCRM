-- @brief Statistics Payment of  the Year
-- @file statistics_payments_month_in_year.sql
SELECT DATE(o_delivered) as day, EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch, count(o_id) AS counts
FROM inventory_orders WHERE date_part('year', o_delivered) <= date_part('year', CURRENT_DATE)
GROUP BY day ORDER BY day;
