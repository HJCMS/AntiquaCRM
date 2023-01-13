-- @brief Default Query for CD's and Vinyl
-- @file query_tab_cdv_main.sql
SELECT cv_id,
  cv_count,
  (CASE WHEN im_id IS NOT NULL THEN true ELSE false END)::BOOLEAN AS cv_image,
  cv_title,
  cv_price::MONEY AS cv_price,
  cv_author,
  cv_eangtin,
  cv_year,
  cv_changed,
  cv_since
FROM inventory_cdvinyl
LEFT JOIN inventory_images ON im_id=cv_id
WHERE @SQL_WHERE_CLAUSE@ ORDER BY @SQL_ORDER_BY@ @SQL_SORTING@ @SQL_LIMIT@;
