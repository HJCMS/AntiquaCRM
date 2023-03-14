-- @brief Default Query for Tab Customers
-- @file query_tab_customers_main.sql
SELECT c_id, c_trusted, c_locked,
  TRIM(CONCAT_WS(' ',c_title,c_firstname,c_lastname)) AS shurename,
  (CASE WHEN c_company IS TRUE THEN c_company_name ELSE '#PR' END) AS company,
  TRIM(CONCAT(c_location,' ',c_country)) AS location_country,
  c_since, c_changed
FROM customers
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
