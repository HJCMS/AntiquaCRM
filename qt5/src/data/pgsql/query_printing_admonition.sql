-- @brief Search Order Payment Information
-- @file query_printing_admonition.sql
SELECT * FROM article_orders WHERE @SQL_WHERE_CLAUSE@ ORDER BY a_order_id;
