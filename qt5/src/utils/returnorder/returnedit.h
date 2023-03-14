// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

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

/**
 * @ingroup _returnorder
 * @brief The ReturnEdit class
 */
class ReturnEdit : public QWidget {
  Q_OBJECT

private:
  PurchaseTable *m_table;
  LineEdit *m_payment_ids;
  ButtonNext *btn_next;

private Q_SLOTS:
  void addPaymentId(const QModelIndex &);

Q_SIGNALS:
  void sendReady();
  void sendMessage(const QString &);

public:
  SerialID *o_id;
  SerialID *o_invoice_id;
  SerialID *o_customer_id;
  DeliveryId *o_delivery;
  AddressInfo *c_postal_address;

  explicit ReturnEdit(QWidget *parent = nullptr);
  void loadArticleData(const QSqlQuery &);
  const QStringList getRefundIds();
};

#endif // RETURN_EDIT_H
