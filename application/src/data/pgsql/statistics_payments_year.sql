-- @brief Statistics Payment of  the Year
-- @file statistics_payments_year.sql
SELECT EXTRACT(EPOCH FROM DATE (o_since)) AS sepoch, count(o_id) AS counts
FROM inventory_orders WHERE @SQL_WHERE_CLAUSE@
GROUP BY DATE(o_since) ORDER BY DATE(o_since);
