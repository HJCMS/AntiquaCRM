// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundsteptwo.h"

RefundStepTwo::RefundStepTwo(QWidget *parent) : RefundingPage{parent} {
  setObjectName("refund_steptwo");
}

bool RefundStepTwo::addDataRecord(qint64 id) {
  qDebug() << Q_FUNC_INFO << id;
  return false;
}
