-- @brief Search Article by Ids
-- @file query_articles_exists.sql
SELECT (CASE WHEN ib_id>0 THEN 'BOOKS' ELSE 'PRINTS' END) AS section
FROM inventory
LEFT JOIN inventory_books ON ib_id=i_id
LEFT JOIN inventory_prints ON ip_id=i_id
WHERE ORDER BY i_id ASC;
