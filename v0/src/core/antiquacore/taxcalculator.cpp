// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taxcalculator.h"

namespace Antiqua {

TaxCalculator::TaxCalculator(qreal price) : p_basePrice{price} {}

const qreal TaxCalculator::plus_vat(qreal vat) const {
  return ((p_basePrice / (100 + vat)) * vat);
}

const qreal TaxCalculator::incl_vat(qreal vat) const {
  return ((p_basePrice * vat) / (100 + vat));
}

}; // namespace Antiqua
