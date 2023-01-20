-- @brief Default Query for Tab Books
-- @file query_tab_prints_main.sql
SELECT ip_id,
  ip_count,
  (CASE WHEN im_id IS NOT NULL THEN true ELSE false END)::BOOLEAN AS ip_image,
  ip_title,
  ip_price::MONEY AS ip_price,
  sl_storage,
  ip_changed,
  ip_author
FROM inventory_prints
LEFT JOIN ref_storage_location ON sl_id=ip_storage
LEFT JOIN inventory_images ON im_id=ip_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
