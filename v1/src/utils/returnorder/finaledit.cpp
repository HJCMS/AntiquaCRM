// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "finaledit.h"

#include <QLabel>
#include <QLayout>

FinalEdit::FinalEdit(QWidget *parent) : QWidget{parent} {
  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  QString txt = tr("Final Page before saving.");
  QLabel *lb1 = new QLabel(txt, this);
  layout->addWidget(lb1, row++, 0, 1, 2);

  txt = tr("If there additional refunds costs, add this to the Input.");
  QLabel *lb2 = new QLabel(txt, this);
  layout->addWidget(lb2, row, 0, 1, 1);

  a_refunds_cost = new PriceEdit(this);
  a_refunds_cost->setObjectName("a_refunds_cost");
  a_refunds_cost->setMinimum(0.00);
  layout->addWidget(a_refunds_cost, row++, 1, 1, 1);

  layout->setRowStretch(row++, 1);

  txt = tr("After applying without errors, this dialog will closed.");
  QLabel *lb3 = new QLabel(txt, this);
  layout->addWidget(lb3, row++, 0, 1, 1);

  setLayout(layout);
}

double FinalEdit::refundsCost() {
  return a_refunds_cost->value().toDouble();
}
