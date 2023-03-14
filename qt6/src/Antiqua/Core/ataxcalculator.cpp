// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ataxcalculator.h"

#include <QLocale>

namespace AntiquaCRM {

ATaxCalculator::ATaxCalculator(double price) : p_origin{price} {}

const QString ATaxCalculator::money(double price) {
  QLocale lc = QLocale::system();
  QString cs = lc.currencySymbol(QLocale::CurrencySymbol);
  return lc.toCurrencyString(price, cs, 2);
}

qreal ATaxCalculator::toAdd(qreal vat) const {
  return ((p_origin / (100)) * vat);
}

qreal ATaxCalculator::getIncl(qreal vat) const {
  return ((p_origin * vat) / (100 + vat));
}

double ATaxCalculator::plus(qreal vat) const {
  return (p_origin + toAdd(vat));
}

double ATaxCalculator::minus(qreal vat) const {
  return (p_origin - getIncl(vat));
}

}; // namespace AntiquaCRM
