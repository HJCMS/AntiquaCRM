-- @brief Lager Positionen
-- @file query_storage_location.sql
SELECT json_build_object('id', sl_id,'storage', sl_storage,'identifier',sl_identifier)
FROM ref_storage_location ORDER BY sl_id;
