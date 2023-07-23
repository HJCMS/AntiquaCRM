// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutpostgresql.h"

AboutPostgreSQL::AboutPostgreSQL(QWidget *parent) : InfoWidget{parent} {
  setInfoIcon(QIcon("://postgresql.png"));
  QUrl url;
  url.setScheme("https");
  url.setHost("www.postgresql.org");
  url.setPath("/");
  setHomepage(url);
  setInfoTitle("The PostgreSQL Licence (PostgreSQL)");
  setInfoText("aboutpostgresql");
}
