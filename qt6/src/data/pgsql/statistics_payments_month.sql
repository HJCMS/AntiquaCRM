-- @brief Statistics payments per month
-- @file statistics_payments_month.sql
SELECT EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch,
  SUM(a_sell_price)::NUMERIC(8,2) AS sell,
  count(a_sell_price) AS counts
FROM inventory_orders LEFT JOIN article_orders ON a_order_id=o_id
WHERE date_part('year',CURRENT_DATE) >= date_part('year',o_delivered) AND
(o_payment_status=1 AND o_order_status=4) OR (o_order_status=6 AND o_payment_status=4)
GROUP BY o_delivered, a_sell_price ORDER BY o_delivered, a_sell_price;
