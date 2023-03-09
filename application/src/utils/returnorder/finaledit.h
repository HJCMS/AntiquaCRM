// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURN_FINAL_EDIT_H
#define RETURN_FINAL_EDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @ingroup utils
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
