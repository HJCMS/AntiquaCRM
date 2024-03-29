// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_REPORTSACTIONGROUP_H
#define ANTIQUACRM_REPORTSACTIONGROUP_H

#include <QActionGroup>
#include <QSignalMapper>
#include <QWidget>

/**
 * @brief Inventory Reports Action group
 * @ingroup _reports
 */
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
