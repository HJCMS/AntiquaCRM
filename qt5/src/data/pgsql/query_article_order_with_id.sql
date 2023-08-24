-- @brief Search Article with ID for Orders system!
-- @see a_type => AntiquaCRM::ArticleType
-- @note This values must always set to prevent invalid MetaTypes in Model Classes!
--  @li a_count=1
--  @li a_payment_id=0
--  @li a_refunds_cost=0.00
-- @file query_article_order_with_id.sql
SELECT 'Office' AS a_provider_id, i_id AS a_article_id,
  CASE i_id WHEN ib_id THEN ib_price WHEN cv_id THEN cv_price WHEN ip_id THEN ip_price WHEN va_id THEN va_price ELSE 0.00 END AS a_price,
  CASE i_id WHEN ib_id THEN ib_price WHEN cv_id THEN cv_price WHEN ip_id THEN ip_price WHEN va_id THEN va_price ELSE 0.00 END AS a_sell_price,
  CASE i_id WHEN ib_id THEN 1 WHEN cv_id THEN 2 WHEN ip_id THEN 3 WHEN va_id THEN 4 ELSE 0 END AS a_type,
  CASE i_id WHEN ib_id THEN 1 ELSE 0 END AS a_tax,
  GREATEST(ib_title, ip_title, cv_title, va_title, NULL) AS a_title,
  1 AS a_count, 0 AS a_payment_id, 0.00 AS a_refunds_cost, CURRENT_TIMESTAMP AS a_modified
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
LEFT JOIN inventory_cdvinyl ON cv_id=i_id
LEFT JOIN inventory_various ON va_id=i_id
WHERE @SQL_WHERE_CLAUSE@ AND (ib_count>0 OR ip_count>0 OR cv_count>0 OR va_count>0) ORDER BY i_id;
