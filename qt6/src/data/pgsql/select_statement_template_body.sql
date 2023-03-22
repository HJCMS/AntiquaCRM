-- @brief Open Templates from ui_template_body
-- @file select_statement_template_body.sql
-- @note Aliases with pattern ('^crm_([a-z_]+)\$') will indicated as template variables!
SELECT o_id AS crm_order_number,
  tb_message, tb_attachment, tb_activ,
  TRIM(CONCAT_WS(' ', c_title, c_firstname, c_lastname)) AS crm_customer_name,
  (CASE
    WHEN c_gender=1 THEN 'Sehr geehrter Herr'
    WHEN c_gender=2 THEN 'Sehr geehrte Frau'
    ELSE 'Sehr geehrte(r)*'
  END)::text AS crm_salutation,
  o_since AS crm_order_since,
  o_invoice_id AS crm_invoice_number,
  o_delivery AS crm_delivery_number,
  CONCAT_WS(':',o_provider_name,o_provider_order_id) AS crm_provider_order
FROM inventory_orders
LEFT JOIN customers ON c_id=o_customer_id
