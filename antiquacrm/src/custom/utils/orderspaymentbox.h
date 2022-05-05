// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSPAYMENTBOX_H
#define ORDERSPAYMENTBOX_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class OrdersPaymentBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_value = 0;
  QLabel *m_info;
  QComboBox *m_box;
  bool paymentStatus;

private Q_SLOTS:
  void indexChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  OrdersPaymentBox(QWidget *parent = nullptr);
  int findIndex(const QString &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

class PaymentStatusDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  OrdersPaymentBox *m_paymentBox;

public:
  explicit PaymentStatusDialog(const QString &search,
                               QWidget *parent = nullptr);
  void setStatus(bool);
  bool status();
};

#endif // ORDERSPAYMENTBOX_H
