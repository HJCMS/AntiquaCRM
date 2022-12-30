// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsactiongroup.h"

#include <QIcon>

StatisticsActionGroup::StatisticsActionGroup(QObject *parent)
    : QActionGroup{parent} {
  setObjectName("tab_statistics_entries");
  p_groups.insert("PaymentsInYear", tr("Payments in year"));
  p_groups.insert("CategoriesInYear", tr("Categories sold per year"));

  m_mapper = new QSignalMapper(this);
  m_mapper->setObjectName("tab_stats_actions");

  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendSelectStats(const QString &)));
}

bool StatisticsActionGroup::loadDataset() {
  QIcon icon(":icons/kchart.png");
  QMapIterator<QString, QString> it(p_groups);
  while (it.hasNext()) {
    it.next();
    QAction *m_ac = addAction(icon, it.value());
    m_mapper->setMapping(m_ac, it.key());
    connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  }
  return true;
}
