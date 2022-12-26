// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGE_RETURNORDER_H
#define STORAGE_RETURNORDER_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QWidget>

class ReturnOrder final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QWidget *m_main;
  QDialogButtonBox *m_btnBox;

  void initData(qint64 orderId);

public:
  explicit ReturnOrder(QWidget *parent = nullptr);
  int exec() override;
  int exec(qint64 orderId = -1);
};

#endif // STORAGE_RETURNORDER_H
