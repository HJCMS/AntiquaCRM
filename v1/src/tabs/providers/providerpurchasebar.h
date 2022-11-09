// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERPURCHASEBAR_H
#define ANTIQUACRM_PROVIDERPURCHASEBAR_H

#include <QStatusBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class ProviderPurchaseBar : public QStatusBar
{
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_check;

Q_SIGNALS:
  void sendCreateOrder();
  void sendCheckArticles();

public Q_SLOTS:
  void enableCreateButton(bool b = false);

public:
  explicit ProviderPurchaseBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PROVIDERPURCHASEBAR_H
