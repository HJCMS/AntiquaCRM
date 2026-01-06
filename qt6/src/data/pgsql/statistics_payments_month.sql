-- @brief Statistics payments per month
-- @file statistics_payments_month.sql
SELECT EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch,
  SUM(a_sell_price)::NUMERIC(8,2) AS sell,
  count(a_sell_price) AS counts
FROM inventory_orders LEFT JOIN article_orders ON a_order_id=o_id
WHERE o_delivered IS NOT NULL
  AND date_part('year',o_delivered) <= date_part('year',CURRENT_DATE)
  AND (o_payment_status IN (1,4) OR o_order_status IN (4,6))
GROUP BY o_delivered, a_sell_price ORDER BY o_delivered, a_sell_price;
