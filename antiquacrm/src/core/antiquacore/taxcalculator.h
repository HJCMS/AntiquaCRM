// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TAXCALCULATOR_ANTIQUACORE_H
#define TAXCALCULATOR_ANTIQUACORE_H

#include "antiqua_global.h"

namespace Antiqua {
/**
 * @brief Steuerrechner
 */
class ANTIQUACORE_EXPORT TaxCalculator final {
private:
  const qreal p_basePrice;

public:
  explicit TaxCalculator(qreal price);
  const qreal plus_vat(qreal vat) const;
  const qreal incl_vat(qreal vat) const;
};
};     // namespace Antiqua
#endif // TAXCALCULATOR_ANTIQUACORE_H
