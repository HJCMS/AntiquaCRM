// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSTABWIDGET_H
#define ANTIQUACRM_CUSTOMERSTABWIDGET_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTabWidget>

/**
 * @brief Inventory Tab widget
 * @ingroup AntiquaCustomers
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
