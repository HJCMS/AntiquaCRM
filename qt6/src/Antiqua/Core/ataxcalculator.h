// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ATAXCALCULATOR_H
#define ANTIQUACRM_ATAXCALCULATOR_H

#include <AGlobal>
#include <QLocale>
#include <QString>

namespace AntiquaCRM {
/**
 * @class ATaxCalculator
 * @brief Sales Tax calculator
 *
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
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ATaxCalculator final {
private:
  const double p_origin;

public:
  /**
   * @param price - money
   */
  explicit ATaxCalculator(double price);

  /**
   * @brief calculate sales tax for current price
   * @param vat - sales tax rate
   */
  qreal toAdd(qreal vat) const;

  /**
   * @brief get included sales tax from current price
   * @param vat - sales tax rate
   */
  qreal getIncl(qreal vat) const;

  /**
   * @brief add sales tax to current price
   * @param vat - sales tax rate
   */
  double plus(qreal vat) const;

  /**
   * @brief Calculate out sales tax from current price
   * @param vat - sales tax rate
   */
  double minus(qreal vat) const;

  /**
   * @brief Convert price to currency string
   * @param value - price
   * @param format
   */
  static const QString money(double value,
        QLocale::CurrencySymbolFormat format = QLocale::CurrencySymbol);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_ATAXCALCULATOR_H
