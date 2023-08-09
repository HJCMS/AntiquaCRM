-- @brief Company Info Settings
-- @file query_company_data.sql
SELECT * FROM antiquacrm_company WHERE ac_class LIKE 'COMPANY_%' ORDER BY ac_sort;
