// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewsactiongroup.h"

#include <AntiquaCRM>
#include <QIcon>

ViewsActionGroup::ViewsActionGroup(QObject *parent) : QActionGroup{parent} {
  setObjectName("tab_views_entries");
  m_mapper = new QSignalMapper(this);
  m_mapper->setObjectName("tab_views_actions");
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendSelectView(const QString &)));
}

bool ViewsActionGroup::loadDataset() {
  QString sql = AntiquaCRM::ASqlFiles::queryStatement("query_views");
  if (sql.isEmpty())
    return false;

  QIcon icon(":icons/view_log.png");
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString tb = q.value("table_name").toString();
      QString co = q.value("comment").toString();
      QAction *m_ac = addAction(icon, co);
      m_mapper->setMapping(m_ac, tb);
      connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
    }
  }
  m_sql->deleteLater();
  return true;
}
