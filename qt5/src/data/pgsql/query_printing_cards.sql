-- @brief Search Printing Cards
-- WHERE i_id IN (105739,116058,96988)
-- @file query_printing_cards.sql
SELECT i_id AS id,
  GREATEST(ib_title,ip_title,cv_title,va_title) AS title,
  GREATEST(ib_author,ip_author,cv_author) AS author,
  GREATEST(ib_year,ip_year,cv_year,va_year) AS year,
  GREATEST(ib_storage,ip_storage,cv_storage,va_storage) AS storage,
  GREATEST(ib_since,ip_since,cv_since,va_since) AS since,
  CASE WHEN ib_storage_compartment IS NULL THEN 'Office' ELSE ib_storage_compartment END AS keywords
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
LEFT JOIN inventory_cdvinyl ON cv_id=i_id
LEFT JOIN inventory_various ON va_id=i_id
WHERE @SQL_WHERE_CLAUSE@
ORDER BY i_id;
