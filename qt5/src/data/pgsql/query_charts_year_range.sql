-- @file query_charts_year_range.sql
SELECT DISTINCT date_part('year', o_delivered)::NUMERIC AS year
FROM inventory_orders WHERE o_delivered IS NOT NULL GROUP BY year ORDER BY year;
