// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksdialog.h"

AbeBooksDialog::AbeBooksDialog(QWidget *parent)
    : AntiquaCRM::UpdateDialog{parent} {
  setMinimumSize(450, 250);
}

bool AbeBooksDialog::prepareAction() { return false; }

void AbeBooksDialog::networkAction() {}

int AbeBooksDialog::exec(const QString &orderId, const QJsonObject &data) {
  p_order_id = orderId;
  p_system_data = data;
  return QDialog::exec();
}
