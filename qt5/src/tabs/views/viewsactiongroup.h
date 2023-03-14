// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_VIEWSACTIONGROUP_H
#define ANTIQUACRM_VIEWSACTIONGROUP_H

#include <QActionGroup>
#include <QSignalMapper>
#include <QWidget>

/**
 * @brief Inventory Views Action group
 * @ingroup tabs
 */
class ViewsActionGroup final : public QActionGroup {
  Q_OBJECT

private:
  QSignalMapper *m_mapper;

Q_SIGNALS:
  void sendSelectView(const QString &viewName);

public:
  explicit ViewsActionGroup(QObject *parent = nullptr);
  bool loadDataset();
};

#endif // ANTIQUACRM_VIEWSACTIONGROUP_H
