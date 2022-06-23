// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebookspurchaser.h"
#include "abebooksconfig.h"

#include <QtWidgets>

AbeBooksPurchaser::AbeBooksPurchaser(QWidget *parent)
    : Antiqua::PurchaserWidget{parent} {
  QStyle *wStyle = style();
  QVBoxLayout *customerLayout = new QVBoxLayout(this);
  setLayout(customerLayout);
}

void AbeBooksPurchaser::setCustomerId(int customerId) {}

void AbeBooksPurchaser::setValue(const QString &objName,
                                 const QVariant &value) {}

const QVariant AbeBooksPurchaser::getValue(const QString &objName) {
  return QVariant();
}
