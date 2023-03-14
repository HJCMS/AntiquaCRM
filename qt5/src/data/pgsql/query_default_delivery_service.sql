-- @brief Query Default Delivary Service
-- @file query_default_delivery_service.sql
SELECT d_srv,d_cid FROM ref_delivery_cost
LEFT JOIN ref_delivery_service ON d_id=d_srv WHERE d_default=true;
