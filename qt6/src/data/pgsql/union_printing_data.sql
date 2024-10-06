-- @brief Query Configuration for Printing
-- @file union_printing_data.sql
SELECT ac_class AS cfg_macro, ac_value AS cfg_value FROM antiquacrm_company UNION
SELECT (CASE WHEN st_tax_type=1 THEN 'PAYMENT_REDUCED'
  WHEN st_tax_type=2 THEN 'PAYMENT_NO_VAT' ELSE 'PAYMENT_NORMAL' END) AS cfg_macro,
  st_tax_value::TEXT AS cfg_value FROM ref_sales_tax ORDER BY cfg_macro;
