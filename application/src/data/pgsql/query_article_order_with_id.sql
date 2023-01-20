-- @brief Search Article for the Order System with Article "id"!
-- @see a_type => AntiquaCRM::ArticleType
-- @file query_article_order_with_id.sql
SELECT 'Internal' AS a_provider_id, COALESCE(ib_id,ip_id) AS a_article_id,
 COALESCE(ib_price,ip_price) AS a_price, COALESCE(ib_price,ip_price) AS a_sell_price,
 1 AS a_count, COALESCE(ib_title,ip_title) AS a_title,
 CASE WHEN (ib_id>0) THEN 1 WHEN (ip_id>0) THEN 3 ELSE 0 END AS a_type
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
WHERE @SQL_WHERE_CLAUSE@ AND (ib_count>0 OR ip_count>0) ORDER BY i_id;
