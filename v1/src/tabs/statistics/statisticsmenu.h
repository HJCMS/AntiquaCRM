// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATISTICS_MENU_H
#define ANTIQUACRM_STATISTICS_MENU_H

#include <QMenu>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

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
