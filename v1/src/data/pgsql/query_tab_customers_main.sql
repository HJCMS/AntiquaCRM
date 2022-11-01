-- @brief Default Query for Tab Customers
-- @file query_tab_customers_main.sql
SELECT c_id AS id,
  c_purchases AS purchaces,
  c_trusted AS trusted,
  c_locked AS locked,
  TRIM(CONCAT_WS(' ',c_title,c_firstname,c_lastname)) AS shurename,
  (CASE WHEN c_company IS TRUE THEN c_company_name ELSE '#PR' END) AS company,
  c_since AS since,
  TRIM(CONCAT_WS(' ',c_country,c_postalcode,c_location,c_street)) AS location
FROM customers
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
