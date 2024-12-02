-- @brief Autofill Query for Article Descriptions
-- @file query_article_descriptions.sql
SELECT json_build_object('id',aes_id,'name',aes_title,'type',aes_type,'description',aes_data,'lang',aes_lang)
FROM article_descriptions WHERE aes_id>0 ORDER BY aes_type,aes_title ASC;
