-- @brief Search and check if Article with ID exists!
-- @see Q_ENUM_NS(AntiquaCRM::ArticleType)
-- @file query_articles_exists.sql
-- @test WHERE i_id IN (1,115456,115911,82885,99999,7) ORDER BY i_id ASC;
SELECT i_id AS article_id,
 GREATEST(ib_type,cv_type,ip_type,va_type)::NUMERIC AS article_identifier,
 fetch_article_identifier(GREATEST(ib_type,cv_type,ip_type,va_type)::NUMERIC) AS article_type,
 GREATEST(ib_title,cv_title,ip_title,va_title)::TEXT AS article_title,
 GREATEST(ib_count,cv_count,ip_count,va_count)::NUMERIC AS article_count
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
LEFT JOIN inventory_cdvinyl ON cv_id=i_id
LEFT JOIN inventory_various ON va_id=i_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY i_id ASC;
