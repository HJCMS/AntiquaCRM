-- @brief Search Order Payment Information
-- @file query_printing_delivery_note.sql
SELECT a_article_id AS aid, a_title AS title, a_count AS crowd
FROM article_orders WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
