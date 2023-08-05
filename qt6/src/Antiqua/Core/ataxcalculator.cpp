// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ataxcalculator.h"
#include "asettings.h"

#include <QLocale>

namespace AntiquaCRM {

ATaxCalculator::ATaxCalculator(double price, int vat_type) : p_origin{price} {
  ASettings cfg;
  cfg.beginGroup("payment");
  if (vat_type == 1) {
    p_vat_value = cfg.value("vat_reduced", 19.0).toDouble();
  } else {
    p_vat_value = cfg.value("vat_normal", 7.0).toDouble();
  }
  cfg.endGroup();
}

void ATaxCalculator::setBillingMode(int mode) {
  AntiquaCRM::SalesTax _mode = static_cast<AntiquaCRM::SalesTax>(mode);
  switch (_mode) {
  case (AntiquaCRM::SalesTax::TAX_INCL):
    p_vat_mode = AntiquaCRM::SalesTax::TAX_INCL;
    break;

  case (AntiquaCRM::SalesTax::TAX_WITH):
    p_vat_mode = AntiquaCRM::SalesTax::TAX_WITH;
    break;

  default:
    p_vat_value = 0.00;
    p_vat_mode = AntiquaCRM::SalesTax::TAX_NOT;
    break;
  }
}

AntiquaCRM::SalesTax ATaxCalculator::getBillingMode() const {
  return p_vat_mode;
}

double ATaxCalculator::salesTaxRate() const { return p_vat_value; }

double ATaxCalculator::toAdd(double vat) const {
  return ((p_origin / (100)) * vat);
}

double ATaxCalculator::getIncl(double vat) const {
  return ((p_origin * vat) / (100 + vat));
}

double ATaxCalculator::vatCosts() const { return getIncl(p_vat_value); }

double ATaxCalculator::netPrice() const { return p_origin; }

double ATaxCalculator::plus(double vat) const {
  return (p_origin + toAdd(vat));
}

double ATaxCalculator::minus(double vat) const {
  return (p_origin - getIncl(vat));
}

double ATaxCalculator::salesPrice() const {
  if (getBillingMode() == AntiquaCRM::SalesTax::TAX_WITH) {
    return plus(p_vat_value);
  }
  return p_origin;
}

const QString ATaxCalculator::money(double value,
                                    QLocale::CurrencySymbolFormat format) {
  QLocale _l = QLocale::system();
  QString _s = _l.currencySymbol(format);
  return _l.toCurrencyString(value, _s, 2);
}

}; // namespace AntiquaCRM
