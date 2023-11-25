// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "refundingpage.h"

RefundingPage::RefundingPage(QWidget *parent) : QWidget{parent} {}

void RefundingPage::setRefundingConfig(const QJsonObject &conf) {
  p_refunds_config = conf;
}

double RefundingPage::refundsCost() {
  return p_refunds_config.value("default").toDouble();
}
