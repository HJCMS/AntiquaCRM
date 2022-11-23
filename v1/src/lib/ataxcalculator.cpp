// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ataxcalculator.h"

namespace AntiquaCRM {

ATaxCalculator::ATaxCalculator(qreal price) : p_basePrice{price} {}

qreal ATaxCalculator::plus_vat(qreal vat) const {
  return ((p_basePrice / (100 + vat)) * vat);
}

qreal ATaxCalculator::incl_vat(qreal vat) const {
  return ((p_basePrice * vat) / (100 + vat));
}

}; // namespace AntiquaCRM
