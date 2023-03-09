// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERPURCHASEBAR_H
#define ANTIQUACRM_PROVIDERPURCHASEBAR_H

#include <AntiquaCRM>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QStatusBar>
#include <QWidget>

/**
 * @brief The ProviderPurchaseBar class
 * @ingroup _providers
 */
class ProviderPurchaseBar : public QStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_customer;
  QPushButton *btn_provider;
  QPushButton *btn_create;
  QPushButton *btn_check;

private Q_SLOTS:
  void createClicked();

Q_SIGNALS:
  void sendViewCustomer();
  void sendProviderAction();
  void sendCreateOrder();
  void sendCheckArticles();

public Q_SLOTS:
  void enableCreateButton(bool b = false);

public:
  explicit ProviderPurchaseBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PROVIDERPURCHASEBAR_H
