/*
 * @brief Update find article types
 * @file update_article_types.sql
 *
 * This update implements the enumerator for article types,
 * see also Q_ENUM_NS(AntiquaCRM::ArticleType)
 *
 */

-- BEGIN remove and polish old structures
ALTER TABLE inventory_books DROP CONSTRAINT IF EXISTS check_ib_type;
ALTER TABLE inventory_books DROP COLUMN IF EXISTS ib_type;

ALTER TABLE inventory_cdvinyl DROP CONSTRAINT IF EXISTS check_cv_type;
ALTER TABLE inventory_cdvinyl DROP COLUMN IF EXISTS cv_type;

ALTER TABLE inventory_prints DROP CONSTRAINT IF EXISTS check_ip_type;
ALTER TABLE inventory_prints DROP COLUMN IF EXISTS ip_type;

ALTER TABLE inventory_various DROP CONSTRAINT IF EXISTS check_va_type;
ALTER TABLE inventory_various DROP COLUMN IF EXISTS va_type;

DROP FUNCTION IF EXISTS fetch_article_identifier;
DROP TABLE IF EXISTS article_types;
DROP FUNCTION IF EXISTS validate_article_type;
DROP TYPE IF EXISTS enum_article_types;
-- END

-- BEGIN creates article types type enumeration
CREATE TYPE enum_article_types AS ENUM ('UNKNOWN','BOOK','MEDIA','PRINTS','OTHER');
COMMENT ON TYPE enum_article_types IS 'AntiquaCRM::ArticleType enumeration';
-- END

-- BEGIN creates article types table interpreter
CREATE TABLE article_types (
  type_identifier enum_article_types NOT NULL,
  article_type numeric(2,0) NOT NULL,
  type_info character varying(80) NOT NULL
);
ALTER TABLE article_types ADD CONSTRAINT article_types_pkey PRIMARY KEY (article_type);
CREATE UNIQUE INDEX article_types_uniq ON article_types USING btree (type_identifier);
COMMENT ON TABLE article_types IS 'This article type table depends on enum_article_types';
-- set type descriptions
INSERT INTO article_types VALUES ('UNKNOWN',0,'article type not specified');
INSERT INTO article_types VALUES ('BOOK',1,'books type');
INSERT INTO article_types VALUES ('MEDIA',2,'media type - CD,Vinyl or DVD');
INSERT INTO article_types VALUES ('PRINTS',3,'prints and stitches type');
INSERT INTO article_types VALUES ('OTHER',4,'other, various or unspecified type');
-- END

-- BEGIN create function queries
-- fetch article identifier width article_type id
CREATE OR REPLACE FUNCTION fetch_article_identifier(numeric(2,0))
  RETURNS TEXT
  LANGUAGE sql
AS $A$
  SELECT type_identifier FROM article_types WHERE article_type=$1;
$A$;

-- validator function
CREATE OR REPLACE FUNCTION validate_article_type(numeric(2,0))
  RETURNS BOOLEAN
  LANGUAGE plpgsql
AS $B$
  DECLARE i_check BOOLEAN;
BEGIN
  SELECT (article_type>0) FROM article_types WHERE article_type=$1 INTO i_check;
  return i_check;
END
$B$;
-- END

-- BEGIN update current inventory tables
ALTER TABLE inventory_books ADD COLUMN IF NOT EXISTS ib_type numeric(2,0) NOT NULL DEFAULT 1;
ALTER TABLE inventory_books ADD CONSTRAINT check_ib_type CHECK (validate_article_type(ib_type));

ALTER TABLE inventory_cdvinyl ADD COLUMN IF NOT EXISTS cv_type numeric(2,0) NOT NULL DEFAULT 2;
ALTER TABLE inventory_cdvinyl ADD CONSTRAINT check_cv_type CHECK (validate_article_type(cv_type));

ALTER TABLE inventory_prints ADD COLUMN IF NOT EXISTS ip_type numeric(2,0) NOT NULL DEFAULT 3;
ALTER TABLE inventory_prints ADD CONSTRAINT check_ip_type CHECK (validate_article_type(ip_type));

ALTER TABLE inventory_various ADD COLUMN IF NOT EXISTS va_type numeric(2,0) NOT NULL DEFAULT 4;
ALTER TABLE inventory_various ADD CONSTRAINT check_va_type CHECK (validate_article_type(va_type));
-- END
