-- @brief Search Article by Ids
-- @file query_articles_exists.sql
-- @see AntiquaCRM::ArticleType
SELECT (CASE WHEN ib_id>0 THEN 1 ELSE 3 END) AS article_type,
 COALESCE(ib_id,ip_id) AS article_id FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY i_id ASC;
