-- @brief Statistics Payment of  the Year
-- @file statistics_payments_year.sql
SELECT
  date_part('doy', o_since) AS doy,
  a_sell_price AS spline
FROM ((public.inventory_orders
LEFT JOIN public.article_orders ON ((a_order_id = o_id)))
LEFT JOIN public.ref_delivery_cost ON ((d_cid = o_delivery_package)))
WHERE (
  (o_payment_status = 1)
  AND (o_order_status = 4)
  AND (date_part('year'::text, o_delivered) = date_part('year'::text, now()))
) ORDER BY o_since;
