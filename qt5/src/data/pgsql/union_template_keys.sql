-- @brief Query Template Macros
-- @file union_template_keys.sql
SELECT ac_class AS macro, ac_info AS title, 'Company' AS category FROM antiquacrm_company
UNION SELECT tk_key AS macro, tk_title AS title, tk_type AS category FROM ui_template_keys ORDER BY macro;
