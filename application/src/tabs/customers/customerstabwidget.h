// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSTABWIDGET_H
#define ANTIQUACRM_CUSTOMERSTABWIDGET_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTabWidget>

/**
 * @brief Inventory Tab widget
 * @ingroup _customers
 */
class CustomersTabWidget : public QTabWidget {
  Q_OBJECT

private:
  AntiquaTabBar *m_tabBar;

public:
  explicit CustomersTabWidget(QWidget *parent = nullptr);
  int placeTab(QWidget *page, const QString &title);
};

#endif // ANTIQUACRM_CUSTOMERSTABWIDGET_H
