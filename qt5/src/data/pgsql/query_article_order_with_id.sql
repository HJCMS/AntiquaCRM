-- @brief Search Article for the Order System with Article "id"!
-- a_count always 1
-- @see a_type => AntiquaCRM::ArticleType
-- @file query_article_order_with_id.sql
SELECT 'Office' AS a_provider_id,
  CASE i_id WHEN ib_id THEN ib_id WHEN cv_id THEN cv_id WHEN ip_id THEN ip_id WHEN va_id THEN va_id ELSE 0 END AS a_article_id,
  CASE i_id WHEN ib_id THEN ib_price WHEN cv_id THEN cv_price WHEN ip_id THEN ip_price WHEN va_id THEN va_price ELSE 0.00 END AS a_price,
  1 AS a_count,
  CASE i_id WHEN ib_id THEN 1 WHEN cv_id THEN 2 WHEN ip_id THEN 3 WHEN va_id THEN 4 ELSE 0 END AS a_type,
  CASE i_id WHEN ib_id THEN 1 ELSE 0 END AS a_tax,
  GREATEST(ib_title, ip_title, cv_title, va_title, NULL) AS a_title,
  0.00 AS a_refunds_cost
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
LEFT JOIN inventory_cdvinyl ON cv_id=i_id
LEFT JOIN inventory_various ON va_id=i_id
WHERE @SQL_WHERE_CLAUSE@ AND (ib_count>0 OR ip_count>0 OR cv_count>0 OR va_count>0) ORDER BY i_id;
