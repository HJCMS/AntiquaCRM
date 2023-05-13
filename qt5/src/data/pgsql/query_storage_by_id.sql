-- @brief Lager Positionen
-- @file query_storage_by_id.sql
SELECT sl_id AS storage_id, sl_storage AS storage_name,
  sl_identifier AS storage_category
FROM ref_storage_location
WHERE @SQL_WHERE_CLAUSE@ ORDER BY sl_id;
