// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURN_ORDER_H
#define RETURN_ORDER_H

#include <AntiquaCRM>
#include <QDialog>
#include <QStackedWidget>
#include <QStatusBar>
#include <QWidget>

class ReturnInfoWidget;
class ReturnEdit;
class FinalEdit;
class ReturnNavigation;

class ReturnOrder final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QStackedWidget *m_stackedWidget;
  ReturnInfoWidget *m_info;
  ReturnEdit *m_medit;
  FinalEdit *m_finalEdit;
  ReturnNavigation *m_btnBox;
  QStatusBar *m_statusBar;
  bool initData(qint64 orderId);

private Q_SLOTS:
  void setStep1();
  void setStep2();
  void saveAndQuit();

public:
  explicit ReturnOrder(QWidget *parent = nullptr);
  int exec() override;
  int exec(qint64 orderId);
};

#endif // RETURN_ORDER_H
