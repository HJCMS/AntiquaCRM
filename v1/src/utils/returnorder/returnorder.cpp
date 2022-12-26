// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returnorder.h"

#include <QDebug>
#include <QLayout>

ReturnOrder::ReturnOrder(QWidget *parent) : QDialog{parent} {
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_main = new QWidget(this);
  layout->addWidget(m_main);

  m_btnBox = new QDialogButtonBox(QDialogButtonBox::Apply, this);
  layout->addWidget(m_btnBox);

  layout->setStretch(0, 1);
  setLayout(layout);
}

void ReturnOrder::initData(qint64 orderId) {
  qDebug() << Q_FUNC_INFO << orderId;
}

int ReturnOrder::exec() {
  qWarning("Don't use this Dialog without OrderID!");
  return QDialog::Rejected;
}

int ReturnOrder::exec(qint64 orderId) {
  if (orderId < 1)
    return QDialog::Rejected;

  m_sql = new AntiquaCRM::ASqlCore(this);

  return QDialog::exec();
}
