-- @brief Default Query for Tab Books
-- @file query_tab_prints_main.sql
SELECT va_id,
  va_count,
  (CASE WHEN im_id IS NOT NULL THEN true ELSE false END)::BOOLEAN AS va_image,
  va_title,
  va_price::MONEY AS va_price,
  sl_storage,
  va_changed,
  va_since
FROM inventory_various
LEFT JOIN ref_storage_location ON sl_id=va_storage
LEFT JOIN inventory_images ON im_id=va_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
