// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "dnbsearch.h"

#include <QDebug>
#include <QDesktopServices>
#include <QMap>

DNBSearch::DNBSearch(QWidget *parent) : QPushButton{parent} {
  setObjectName("dnb_search_button");
  setIcon(QIcon(":/icons/network.png"));
  setText(tr("Search"));
  setToolTip(tr("Opens the search page of the German National "
                "Library in your web browser."));
  setStatusTip(tr("Link: German National Library"));

  p_type = "all";

  QMap<QString, QString> _map;
  _map.insert("all", tr("Search in everything"));
  _map.insert("tit", tr("Title search"));
  _map.insert("nam", tr("Authors search"));
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

const QIcon DNBSearch::icon() const {
  return QIcon(":/icons/action_search.png");
}

const QUrl DNBSearch::home() const {
  QUrl _url;
  _url.setScheme("https");
  _url.setHost("katalog.dnb.de");
  _url.setPath("/DE/home.html");
  return _url;
}

const QUrl DNBSearch::list() const {
  QUrl _url;
  _url.setScheme("https");
  _url.setHost("katalog.dnb.de");
  _url.setPath("/DE/list.html");
  return _url;
}

void DNBSearch::prepareAction(QAction *action) {
  if (action == nullptr)
    return;

  p_type = action->objectName();
  if (p_type.length() < 3) {
    p_type.clear();
    return;
  }
  emit sendQuery();
}

void DNBSearch::sendQuery(const QUrlQuery &query) {
  QUrlQuery _query(query);
  _query.addQueryItem("v", "plist");

  QUrl _url = list();
  _url.setQuery(_query);
  if (_url.isValid())
    QDesktopServices::openUrl(_url);
  else
    QDesktopServices::openUrl(home());
}

const QString DNBSearch::getSearchType() { return p_type; }

const QStringList DNBSearch::params() {
  return QStringList({"all", "tit", "nam", "num"});
}
