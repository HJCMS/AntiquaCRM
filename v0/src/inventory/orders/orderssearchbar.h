// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERS_SEARCHBAR_H
#define INVENTORY_ORDERS_SEARCHBAR_H

#include <QFrame>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

class OrdersSearchBar : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:

public:
  explicit OrdersSearchBar(QWidget *parent = nullptr);
};

#endif // INVENTORY_ORDERS_SEARCHBAR_H
