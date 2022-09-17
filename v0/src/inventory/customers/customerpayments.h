// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CUSTOMER_PAYMENTS_H
#define CUSTOMER_PAYMENTS_H

#include <QObject>
#include <QTableWidget>
#include <QWidget>

class CustomerPayments final : public QTableWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit CustomerPayments(QWidget *parent = nullptr);
  void restore();
};

#endif // CUSTOMER_PAYMENTS_H
