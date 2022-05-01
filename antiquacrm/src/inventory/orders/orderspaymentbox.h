// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSPAYMENTBOX_H
#define ORDERSPAYMENTBOX_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

class OrdersPaymentBox : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int paymentStatus READ getPaymentStatus WRITE setPaymentStatus
                 NOTIFY paymentStatusChanged)

private:
  bool paymentStatus;

private Q_SLOTS:
  void indexChanged(int);

Q_SIGNALS:
  void paymentStatusChanged(bool);

public Q_SLOTS:
  void setPaymentStatus(bool);

public:
  OrdersPaymentBox(QWidget *parent = nullptr);
  bool getPaymentStatus();
};

class PaymentStatusDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  OrdersPaymentBox *m_box;

public:
  explicit PaymentStatusDialog(const QString &search,
                               QWidget *parent = nullptr);
  void setStatus(bool);
  bool status();
};

#endif // ORDERSPAYMENTBOX_H
