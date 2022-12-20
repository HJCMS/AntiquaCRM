// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PAYMENTSINYEAR_H
#define ANTIQUACRM_PAYMENTSINYEAR_H

#include <QDate>
#include <QObject>
#include <QWidget>

class PaymentsInYear final : public QWidget {
  Q_OBJECT

public:
  explicit PaymentsInYear(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PAYMENTSINYEAR_H
