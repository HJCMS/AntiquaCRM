// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreunddialog.h"

BuchfreundDialog::BuchfreundDialog(QWidget *parent)
    : AntiquaCRM::UpdateDialog{parent} {
  setMinimumSize(450, 250);
}

bool BuchfreundDialog::prepareAction() { return false; }

void BuchfreundDialog::networkAction() {}

int BuchfreundDialog::exec(const QString &orderId, const QJsonObject &data) {
  p_order_id = orderId;
  p_system_data = data;

  return QDialog::exec();
}
