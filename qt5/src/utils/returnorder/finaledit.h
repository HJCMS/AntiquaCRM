// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef RETURN_FINAL_EDIT_H
#define RETURN_FINAL_EDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @ingroup _returnorder
 * @brief The FinalEdit class
 */
class FinalEdit : public QWidget {
  Q_OBJECT

private:
  PriceEdit *a_refunds_cost;

public:
  explicit FinalEdit(QWidget *parent = nullptr);
  double refundsCost();
};

#endif // RETURN_FINAL_EDIT_H
