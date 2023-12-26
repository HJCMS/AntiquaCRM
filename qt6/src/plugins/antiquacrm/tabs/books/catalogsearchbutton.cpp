// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "catalogsearchbutton.h"

#include <QDebug>
#include <QDesktopServices>

CatalogSearchButton::CatalogSearchButton(QWidget *parent)
    : QPushButton{parent}, p_type{"all"} {
  setObjectName("dnb_search_button");
  setIcon(AntiquaCRM::antiquaIcon("network"));
  setText(tr("Search") + " (Beta)");
  setToolTip(tr("Opens the search page of the German National "
                "Library in your web browser."));
  setStatusTip(tr("Link: German National Library"));

  // NOTE Deprecated
  QMap<QString, QString> _map;
  _map.insert("all", tr("Search in everything"));
  _map.insert("tit", tr("Title search"));
  _map.insert("nam", tr("Authors search"));
  _map.insert("tit+nam", tr("Title and Author search"));
  _map.insert("num", tr("ISBN search"));

  m_menu = new QMenu(this);
  QMap<QString, QString>::const_iterator i = _map.constBegin();
  while (i != _map.constEnd()) {
    if (params().contains(i.key())) {
      QAction *ac = m_menu->addAction(icon(), i.value());
      ac->setObjectName(i.key());
      ac->setStatusTip(i.value());
    }
    ++i;
  }
  setMenu(m_menu);

  connect(m_menu, SIGNAL(triggered(QAction *)), SLOT(prepareAction(QAction *)));
}

const QIcon CatalogSearchButton::icon() const {
  return AntiquaCRM::antiquaIcon("action-search");
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

void CatalogSearchButton::prepareAction(QAction *action) {
  if (action == nullptr)
    return;

  p_type = action->objectName();
  if (p_type.length() < 3) {
    p_type.clear();
    return;
  }
  emit sendQuery();
}

void CatalogSearchButton::openLink(const QUrlQuery &query) {
  QUrlQuery _query(query);
  _query.addQueryItem("v", "plist");

  QUrl _url = list();
  _url.setQuery(_query);
  if (_url.isValid())
    QDesktopServices::openUrl(_url);
  else
    QDesktopServices::openUrl(home());
}

const QMap<CatalogSearchButton::Option, QString>
CatalogSearchButton::options() const {
  QMap<CatalogSearchButton::Option, QString> _m;
  _m.insert(Option::ANY, tr("Search in everything"));
  _m.insert(Option::TITLE, tr("Title search"));
  _m.insert(Option::AUTHOR, tr("Authors search"));
  _m.insert(Option::TITAUT, tr("Title and Author search"));
  _m.insert(Option::NUMERIC, tr("ISBN search"));
  return _m;
}

const QString CatalogSearchButton::getSearchType() { return p_type; }

const QStringList CatalogSearchButton::params() {
  return QStringList({"all", "tit", "nam", "num", "tit+nam"});
}
