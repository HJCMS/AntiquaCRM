-- @brief Query Default Delivery Service
-- @note In second query d_name must alaways NULL
-- @file union_default_delivery.sql
SELECT d_id, d_name, 0 AS d_index FROM ref_delivery_service UNION
SELECT d_srv AS d_id, NULL AS d_name, d_srv AS d_index
  FROM ref_delivery_cost WHERE d_default=true ORDER BY d_id;
