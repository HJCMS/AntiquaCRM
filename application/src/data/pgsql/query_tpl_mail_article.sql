-- @brief Read Article date for E-Mail Templates
-- @file query_tpl_mail_article.sql
SELECT a_article_id,
  CONCAT('{"count":', a_count, ',"article":', a_article_id,',"title":"',
    GREATEST(ib_title, cv_title, ip_title, va_title, a_title), '"}')::jsonb AS a_article_display
FROM article_orders
LEFT JOIN inventory_books ON ib_id=a_article_id
LEFT JOIN inventory_cdvinyl ON cv_id=a_article_id
LEFT JOIN inventory_prints ON ip_id=a_article_id
LEFT JOIN inventory_various ON va_id=a_article_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
