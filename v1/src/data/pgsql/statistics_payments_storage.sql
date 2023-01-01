-- @brief Statistics Payment of  the Year
-- @file statistics_payments_storage.sql
SELECT sl_identifier, count(sl_id) AS counts,
  sum(a_sell_price) AS volume
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
LEFT JOIN inventory_books ON ib_id=a_article_id
LEFT JOIN inventory_prints ON ip_id=a_article_id
LEFT JOIN ref_storage_location ON sl_id=COALESCE(ib_storage,ip_storage)
WHERE date_part('year', o_since) = @SQL_WHERE_CLAUSE@ GROUP BY sl_id ORDER BY counts;
