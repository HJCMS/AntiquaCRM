-- @brief Statistics payments per month
-- @file statistics_payments_month.sql
SELECT EXTRACT(EPOCH FROM DATE (o_delivered)) AS sepoch,
  SUM(a_sell_price)::NUMERIC(8,2) AS sell, count(o_id) AS counts
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
WHERE ((o_payment_status=1 AND o_order_status=4)
    OR (o_order_status=6 AND o_payment_status=4)
  ) AND date_part('year', o_delivered) <= date_part('year', CURRENT_DATE)
GROUP BY o_delivered ORDER BY o_delivered;
