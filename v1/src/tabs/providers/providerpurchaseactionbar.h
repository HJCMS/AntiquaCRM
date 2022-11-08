// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERPURCHASEACTIONBAR_H
#define ANTIQUACRM_PROVIDERPURCHASEACTIONBAR_H

#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class ProviderPurchaseActionBar : public QToolBar
{
  Q_OBJECT

private:
  QLineEdit *m_edit;
  QLabel *m_buyer;
  QPushButton *m_btn;

Q_SIGNALS:
  void sendCheckArticles();

public:
  explicit ProviderPurchaseActionBar(QWidget *parent = nullptr);
  void setCustomer(const QString &buyer, qint64 cid = 0);
};

#endif // ANTIQUACRM_PROVIDERPURCHASEACTIONBAR_H
