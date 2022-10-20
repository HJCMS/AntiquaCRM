-- @brief Autofill Query Keywords
-- @file query_keywords.sql
SELECT json_build_object('name',ci_name) FROM categories_intern WHERE ci_company_usage=true ORDER BY ci_name ASC;
