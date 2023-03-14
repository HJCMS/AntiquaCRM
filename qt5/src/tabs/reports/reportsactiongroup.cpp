// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsactiongroup.h"

ReportsActionGroup::ReportsActionGroup(QObject *parent) : QActionGroup{parent} {
  setObjectName("tab_reports_entries");
  m_mapper = new QSignalMapper(this);
  m_mapper->setObjectName("tab_reports_actions");
  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendSelectView(const QString &)));
}

bool ReportsActionGroup::loadDataset() {
  // <Action,Title>
  QMap<QString, QString> map;
  map.insert("MONTHLY_REPORT",tr("Monthly report"));

  QIcon icon(":icons/view_log.png");
  QMapIterator<QString, QString> it(map);
  while (it.hasNext()) {
    it.next();
    QAction *m_ac = addAction(icon, it.value());
    m_mapper->setMapping(m_ac, it.key());
    connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  }
  return true;
}
