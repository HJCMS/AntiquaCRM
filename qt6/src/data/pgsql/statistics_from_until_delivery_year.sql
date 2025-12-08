-- @file statistics_from_until_delivery_year.sql
SELECT MIN(DATE(o_delivered)) AS min, MAX(DATE(o_delivered)) AS max
FROM inventory_orders WHERE o_delivered IS NOT NULL ORDER BY min,max;
