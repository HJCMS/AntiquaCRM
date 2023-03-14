// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsmenu.h"

#include <QIcon>

StatisticsMenu::StatisticsMenu(QWidget *parent)
    : QMenu{tr("Statistics"), parent} {
  setObjectName("tab_statistics_entries");
  setIcon(QIcon(":icons/kchart.png"));

  m_mapper = new QSignalMapper(this);
  m_mapper->setObjectName("tab_stats_actions");

  m_thisYear = addMenu(icon(), tr("This Year"));
  m_lastYear = addMenu(icon(), tr("Last Year"));

  addSeparator();

  ac_custom = addAction(icon(), tr("Extended query"));

  m_mapper->setMapping(ac_custom, "default_page");
  connect(ac_custom, SIGNAL(triggered()), m_mapper, SLOT(map()));

  setThisYearGroup();
  setLastYearGroup();

  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendSelectStats(const QString &)));
}

void StatisticsMenu::setThisYearGroup() {
  QActionGroup *m_acg = new QActionGroup(m_thisYear);
  QMap<QString, QString> m;
  m.insert("DailyOrdersThisYear", tr("Daily average"));
  m.insert("SelledCategoriesThisYear", tr("Categories sales"));
  m.insert("ProviderOrderThisYear", tr("Provider sales"));
  m.insert("ProviderVolumeThisYear", tr("Provider payments volume"));

  QMapIterator<QString, QString> it(m);
  while (it.hasNext()) {
    it.next();
    QAction *m_ac = m_acg->addAction(icon(), it.value());
    m_mapper->setMapping(m_ac, it.key());
    connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  }
  m_thisYear->addActions(m_acg->actions());
}

void StatisticsMenu::setLastYearGroup() {
  QActionGroup *m_acg = new QActionGroup(m_lastYear);
  QMap<QString, QString> m;
  m.insert("DailyOrdersLastYear", tr("Daily average"));
  m.insert("SelledCategoriesLastYear", tr("Categories sales"));
  m.insert("ProviderOrderLastYear", tr("Provider sales"));
  m.insert("ProviderVolumeLastYear", tr("Provider sales volume"));

  QMapIterator<QString, QString> it(m);
  while (it.hasNext()) {
    it.next();
    QAction *m_ac = m_acg->addAction(icon(), it.value());
    m_mapper->setMapping(m_ac, it.key());
    connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  }
  m_lastYear->addActions(m_acg->actions());
}
