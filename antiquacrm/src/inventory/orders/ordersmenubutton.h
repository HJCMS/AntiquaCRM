// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSMENUBUTTON_ORDERS_H
#define ORDERSMENUBUTTON_ORDERS_H

#include <QAction>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class OrdersMenuButton final : public QPushButton {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QAction *ac_hasClosed;
  QAction *ac_noPayment;
  QAction *ac_noDeliver;

private Q_SLOTS:
  void setQueryClosedOrders();
  void setQueryNoPayments();
  void setQueryNoDelivery();

Q_SIGNALS:
  void sendQueryAction(const QString &query);

public:
  explicit OrdersMenuButton(QWidget *parent = nullptr);
};

#endif // ORDERSMENUBUTTON_ORDERS_H
