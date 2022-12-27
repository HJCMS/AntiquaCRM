// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURN_EDIT_H
#define RETURN_EDIT_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QModelIndex>
#include <QObject>
#include <QSqlQuery>
#include <QWidget>

class PurchaseTable;
class ButtonNext;

class ReturnEdit : public QWidget {
  Q_OBJECT

private Q_SLOTS:
  void addPaymentId(const QModelIndex &);
  void checkBeforeNext();

Q_SIGNALS:
  void sendPaymentIds(const QStringList &);
  void sendMessage(const QString &);

public:
  SerialID *o_id;
  SerialID *o_invoice_id;
  SerialID *o_customer_id;
  DeliveryId *o_delivery;
  AddressInfo *c_postal_address;
  PurchaseTable *m_table;
  LineEdit *m_payment_ids;
  ButtonNext *btn_next;

  explicit ReturnEdit(QWidget *parent = nullptr);
  void loadArticleData(const QSqlQuery &);
};

#endif // RETURN_EDIT_H
