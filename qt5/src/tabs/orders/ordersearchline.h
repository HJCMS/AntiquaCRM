// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ORDERSEARCHLINE_H
#define ANTIQUACRM_ORDERSEARCHLINE_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Orders Customer Search Line Edit
 * @ingroup _orders
 */
class OrderSearchLine final : public SearchLineEdit {
  Q_OBJECT

public:
  OrderSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_ORDERSEARCHLINE_H
