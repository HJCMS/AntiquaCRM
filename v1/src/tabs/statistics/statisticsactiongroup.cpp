// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsactiongroup.h"

#include <QIcon>

StatisticsActionGroup::StatisticsActionGroup(QObject *parent)
    : QActionGroup{parent} {
  setObjectName("tab_statistics_entries");
  p_groups.insert("default_page", tr("Customized Statistics"));
  p_groups.insert("PaymentsLastYear", tr("Payments last year"));
  p_groups.insert("CategoriesLastYear", tr("Categories sales last year"));
  p_groups.insert("PrOrdersLastYear", tr("Provider sales last year"));

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
