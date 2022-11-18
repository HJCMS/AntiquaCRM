// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ATAXCALCULATOR_H
#define ANTIQUACRM_ATAXCALCULATOR_H

#include <AGlobal>

namespace AntiquaCRM {
/**
 * @brief Steuerrechner
 */
class ANTIQUACRM_LIBRARAY ATaxCalculator final {
private:
  const qreal p_basePrice;

public:
  explicit ATaxCalculator(qreal price);
  const qreal plus_vat(qreal vat) const;
  const qreal incl_vat(qreal vat) const;
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_ATAXCALCULATOR_H
