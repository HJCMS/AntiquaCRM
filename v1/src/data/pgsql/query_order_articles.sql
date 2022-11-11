-- @brief Search Payment Article with Order Id
-- @file query_order_articles.sql
SELECT a_provider_id, a_article_id, a_price, a_sell_price, a_count, a_type, a_title
FROM article_orders WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
