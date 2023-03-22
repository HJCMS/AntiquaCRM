-- @brief Monthly Categories
-- @file statistics_monthly_categories.sql
-- WHERE date_part('year', o_since) = 2022 AND date_part('month', o_since) = 6
SELECT sl_identifier,
  count(sl_id) AS counts
FROM inventory_orders
LEFT JOIN article_orders ON a_order_id=o_id
LEFT JOIN inventory_books ON ib_id=a_article_id
LEFT JOIN inventory_prints ON ip_id=a_article_id
LEFT JOIN ref_storage_location ON sl_id=COALESCE(ib_storage,ip_storage)
WHERE @SQL_WHERE_CLAUSE@ GROUP BY sl_id ORDER BY counts;
