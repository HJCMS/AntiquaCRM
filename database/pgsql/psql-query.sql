SELECT a.o_id,a.o_since,a.o_order_status,a.o_payment_status,
  CASE WHEN c.c_company=true THEN c.c_company_name ELSE concat_ws(' ',c.c_firstname,c.c_lastname) END AS costumer,
  d.d_name,a.o_locked,a.o_closed,age(CURRENT_TIMESTAMP,o_since) AS age
FROM inventory_orders AS a
LEFT JOIN costumers AS c ON c.c_id=a.o_costumer_id
LEFT JOIN ref_delivery_service AS d ON d.d_id=a.o_delivery_service
WHERE a.o_closed=false ORDER BY a.o_since DESC;
