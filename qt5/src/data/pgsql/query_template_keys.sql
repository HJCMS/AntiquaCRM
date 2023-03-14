-- @brief Open Templates from ui_template_body
-- @file query_template_keys.sql
SELECT * FROM ui_template_keys WHERE @SQL_WHERE_CLAUSE@ ORDER BY (tk_key,tk_id);
