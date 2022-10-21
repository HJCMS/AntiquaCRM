SELECT
  json_build_object('lang', 'de','name','Deutschland') AS country,
  json_build_object('plz', p_plz,'location', p_location,'state', p_state) AS codes
  FROM ref_postalcodes_de
UNION ALL
SELECT
  json_build_object('lang', 'at','name','Österreich') AS country,
  json_build_object('plz', p_plz,'location', p_location,'state', p_state) AS codes
  FROM ref_postalcodes_at
UNION ALL
SELECT
  json_build_object('lang', 'ch','name','Schweiz') AS country,
  json_build_object('plz', p_plz,'location', p_location,'state', p_state) AS codes
  FROM ref_postalcodes_ch;
