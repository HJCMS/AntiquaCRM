-- @brief Statistics Payment of  the Year
-- @file statistics_payments_year.sql
SELECT EXTRACT(EPOCH FROM DATE (o_since)) AS since
FROM ((public.inventory_orders
LEFT JOIN public.article_orders ON ((a_order_id = o_id)))
LEFT JOIN public.ref_delivery_cost ON ((d_cid = o_delivery_package)))
WHERE ((o_payment_status = 1) AND (o_order_status = 4)
  AND (date_part('year', o_delivered) = @SQL_WHERE_CLAUSE@)
) ORDER BY o_since;
