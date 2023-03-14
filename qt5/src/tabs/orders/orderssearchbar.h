// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ORDERSSEARCHBAR_H
#define ANTIQUACRM_ORDERSSEARCHBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class OrderSearchLine;

/**
 * @brief Inventory Orders Search bar
 * @ingroup _orders
 */
class OrdersSearchBar final : public TabSearchBar {
  Q_OBJECT

private:
  QString searchField;
  QString searchText;
  AntiquaComboBox *m_selectFilter;
  OrderSearchLine *m_lineEdit;
  QPushButton *m_searchBtn;
  AntiquaCRM::OrderStatus currentStatus;

public Q_SLOTS:
  void setSearch() override;
  void setFilter(int) override;
  void setFilterFocus() override;
  void setClearAndFocus() override;
  void setSearchFocus() override;

public:
  explicit OrdersSearchBar(QWidget *parent = nullptr);
  int searchLength() override;
  const QString getSearchStatement() override;
};

#endif // ANTIQUACRM_ORDERSSEARCHBAR_H
