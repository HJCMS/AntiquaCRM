-- @brief Query Template Macros
-- @file union_template_values.sql
SELECT ac_class AS tpl_macro, ac_value AS tpl_value FROM antiquacrm_company
UNION SELECT tk_key AS tpl_macro, CONCAT('@',tk_key,'@') AS tpl_value
  FROM ui_template_keys WHERE tk_type='STATIC' ORDER BY tpl_macro;
