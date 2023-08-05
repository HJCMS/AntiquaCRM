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
  double vat = 19.0;
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
  double p_origin;
  double p_vat_value;
  AntiquaCRM::SalesTax p_vat_mode;

public:
  /**
   * @param price - money
   */
  explicit ATaxCalculator(double price, int vat_type);

  /**
   * @brief Set Current VAT Mode
   * @param mode - AntiquaCRM::SalesTax
   */
  void setBillingMode(int mode);

  /**
   * @brief Current VAT Billing mode
   * @sa AntiquaCRM::SalesTax
   */
  AntiquaCRM::SalesTax getBillingMode() const;

  /**
   * @brief Current sales tax rate
   */
  double salesTaxRate() const;

  /**
   * @brief calculate sales tax for current price
   * @param vat - sales tax rate
   */
  double toAdd(double vat) const;

  /**
   * @brief get included sales tax from current price
   * @param vat - sales tax rate
   */
  double getIncl(double vat) const;

  /**
   * @brief calculated VAT Value from price
   */
  double vatCosts() const;

  /**
   * @brief net price without any VAT calculation
   */
  double netPrice() const;

  /**
   * @brief add sales tax to current price
   * @param vat - sales tax rate
   */
  double plus(double vat) const;

  /**
   * @brief Calculate out sales tax from current price
   * @param vat - sales tax rate
   */
  double minus(double vat) const;

  /**
   * @brief Returning sales price, calculated by settings
   */
  double salesPrice() const;

  /**
   * @brief Convert price to currency string
   * @param value - price
   * @param format
   */
  static const QString
  money(double value,
        QLocale::CurrencySymbolFormat format = QLocale::CurrencySymbol);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_ATAXCALCULATOR_H
