-- @brief Default Query for Tab Books
-- @file query_tab_books_main.sql
SELECT ib_id,
  ib_count,
  (CASE WHEN im_id IS NOT NULL THEN true ELSE false END)::BOOLEAN AS ib_image,
  ib_title,
  ib_price::MONEY AS ib_price,
  sl_storage,
  ib_changed,
  ib_author,
  ib_publisher,
  ib_year,
  ib_since
FROM inventory_books
LEFT JOIN ref_storage_location ON sl_id=ib_storage
LEFT JOIN inventory_images ON im_id=ib_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
