// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "catalogsearchbutton.h"

#include <QDesktopServices>

CatalogSearchButton::CatalogSearchButton(QWidget* parent) : QPushButton{parent}, p_type{"all"} {
  setObjectName("dnb_search_button");
  setIcon(AntiquaCRM::antiquaIcon("network"));
  setText(tr("DNB Website"));
  setToolTip(
      tr("Opens the search page of the German National "
         "Library in your web browser."));
  setStatusTip(tr("Link: German National Library"));
  connect(this, SIGNAL(clicked()), SLOT(openSearchPage()));
}

const QIcon CatalogSearchButton::icon() const {
  return AntiquaCRM::antiquaIcon("action-search");
}

void CatalogSearchButton::openSearchPage() {
  QDesktopServices::openUrl(home());
}

const QUrl CatalogSearchButton::home() const {
  QUrl _url;
  _url.setScheme("https");
  _url.setHost("katalog.dnb.de");
  _url.setPath("/DE/home.html");
  return _url;
}

const QUrl CatalogSearchButton::list() const {
  QUrl _url;
  _url.setScheme("https");
  _url.setHost("katalog.dnb.de");
  _url.setPath("/DE/list.html");
  return _url;
}

const QMap<CatalogSearchButton::Option, QString> CatalogSearchButton::options() const {
  QMap<CatalogSearchButton::Option, QString> _m;
  _m.insert(Option::ANY, tr("Search in everything"));
  _m.insert(Option::TITLE, tr("Title search"));
  _m.insert(Option::AUTHOR, tr("Authors search"));
  _m.insert(Option::TITAUT, tr("Title and Authors search"));
  _m.insert(Option::NUMERIC, tr("ISBN search"));
  return _m;
}

const QString CatalogSearchButton::getSearchType() {
  return p_type;
}

const QStringList CatalogSearchButton::params() {
  return QStringList({"all", "tit", "nam", "num", "tit+nam"});
}
