// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_REPORTSACTIONGROUP_H
#define ANTIQUACRM_REPORTSACTIONGROUP_H

#include <QActionGroup>
#include <QSignalMapper>
#include <QWidget>

class ReportsActionGroup : public QActionGroup {
  Q_OBJECT

private:
  QSignalMapper *m_mapper;

Q_SIGNALS:
  void sendSelectView(const QString &viewName);

public:
  explicit ReportsActionGroup(QObject *parent = nullptr);
  bool loadDataset();
};

#endif // ANTIQUACRM_REPORTSACTIONGROUP_H
