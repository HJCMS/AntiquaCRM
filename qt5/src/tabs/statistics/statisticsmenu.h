// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_STATISTICS_MENU_H
#define ANTIQUACRM_STATISTICS_MENU_H

#include <QMenu>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

/**
 * @brief The StatisticsMenu class
 * @ingroup _chartstat
 */
class StatisticsMenu final : public QMenu {
  Q_OBJECT

private:
  QAction *ac_custom;
  QMenu *m_thisYear;
  QMenu *m_lastYear;
  QSignalMapper *m_mapper;
  void setThisYearGroup();
  void setLastYearGroup();

Q_SIGNALS:
  void sendSelectStats(const QString &);

public:
  explicit StatisticsMenu(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_STATISTICS_MENU_H
