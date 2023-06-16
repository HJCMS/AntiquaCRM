// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ataxcalculator.h"

#include <QLocale>

namespace AntiquaCRM {

ATaxCalculator::ATaxCalculator(double price) : p_origin{price} {}

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

const QString ATaxCalculator::money(double  value,
                             QLocale::CurrencySymbolFormat format) {
  QLocale _l = QLocale::system();
  QString _s = _l.currencySymbol(format);
  return _l.toCurrencyString(value, _s, 2);
}

}; // namespace AntiquaCRM
