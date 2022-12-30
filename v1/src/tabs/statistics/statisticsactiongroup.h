// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATISTICS_ACTIONGROUP_H
#define ANTIQUACRM_STATISTICS_ACTIONGROUP_H

#include <QActionGroup>
#include <QObject>
#include <QSignalMapper>

class StatisticsActionGroup final : public QActionGroup {
  Q_OBJECT

private:
  QMap<QString, QString> p_groups;
  QSignalMapper *m_mapper;

Q_SIGNALS:
  void sendSelectStats(const QString &viewName);

public:
  explicit StatisticsActionGroup(QObject *parent = nullptr);
  bool loadDataset();
};

#endif // ANTIQUACRM_STATISTICS_ACTIONGROUP_H
