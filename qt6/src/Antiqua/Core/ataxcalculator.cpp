// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ataxcalculator.h"

#include <QLocale>

namespace AntiquaCRM {

ATaxCalculator::ATaxCalculator(double price, int vat_type)
    : p_origin{price}, p_vat_type{vat_type} {}

qreal ATaxCalculator::toAdd(qreal vat) const {
  return ((p_origin / (100)) * vat);
}

qreal ATaxCalculator::getIncl(qreal vat) const {
  return ((p_origin * vat) / (100 + vat));
}

void ATaxCalculator::setBillingMode(AntiquaCRM::SalesTax mode) {
  switch (mode) {
  case (AntiquaCRM::SalesTax::TAX_NOT):
    p_vat_mode = AntiquaCRM::SalesTax::TAX_NOT;
    break;

  case (AntiquaCRM::SalesTax::TAX_INCL):
    p_vat_mode = AntiquaCRM::SalesTax::TAX_INCL;
    break;

  case (AntiquaCRM::SalesTax::TAX_WITH):
    p_vat_mode = AntiquaCRM::SalesTax::TAX_WITH;
    break;

  default:
    p_vat_mode = AntiquaCRM::SalesTax::TAX_INCL;
    break;
  }
}

AntiquaCRM::SalesTax ATaxCalculator::getBillingMode() const {
  return p_vat_mode;
}

int ATaxCalculator::vatType() const { return p_vat_type; }

double ATaxCalculator::netprice() const { return p_origin; }

double ATaxCalculator::plus(qreal vat) const { return (p_origin + toAdd(vat)); }

double ATaxCalculator::minus(qreal vat) const {
  return (p_origin - getIncl(vat));
}

const QString ATaxCalculator::money(double value,
                                    QLocale::CurrencySymbolFormat format) {
  QLocale _l = QLocale::system();
  QString _s = _l.currencySymbol(format);
  return _l.toCurrencyString(value, _s, 2);
}

}; // namespace AntiquaCRM
