// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ATAXCALCULATOR_H
#define ANTIQUACRM_ATAXCALCULATOR_H

#include <AGlobal>
#include <QString>

namespace AntiquaCRM {
/**
 * @brief Sales Tax calculator
 * @code
  qreal vat = 19.0;
  double salePrice = 143.45;
  AntiquaCRM::ATaxCalculator calcTax(salePrice);
  double totalPrice = calcTax.plus(vat);
  qDebug() << "Sales Price:" << calcTax.money(salePrice)
    << "plus" << vat << "% VAT:" << calcTax.toAdd(vat)
    << "Total Price:" << calcTax.money(totalPrice);

  double nettoPrice = calcTax.minus(vat);
  qDebug() << "Sales Price:" << calcTax.money(salePrice)
    << "incl" << vat << "% VAT:" << calcTax.getIncl(vat)
    << "Total Price:" << calcTax.money(nettoPrice);
 * @endcode
 */
class ANTIQUACRM_LIBRARY ATaxCalculator final {
private:
  const double p_origin;

public:
  explicit ATaxCalculator(double price);

  /**
   * @brief Convert price to currency string
   * @param price
   */
  const QString money(double price);

  /**
   * @brief calculate sales tax for current price
   * @param vat - sales tax rate
   * @return Sales tax value to be add ...
   */
  qreal toAdd(qreal vat) const;

  /**
   * @brief get included sales tax from current price
   * @param vat - sales tax rate
   * @return Included sales tax value ...
   */
  qreal getIncl(qreal vat) const;

  /**
   * @brief add sales tax to current price
   * @param vat - sales tax rate
   * @return Price plus sales tax
   */
  double plus(qreal vat) const;

  /**
   * @brief Calculate out sales tax from current price
   * @param vat - sales tax rate
   * @return Price minus sales tax
   */
  double minus(qreal vat) const;
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_ATAXCALCULATOR_H
