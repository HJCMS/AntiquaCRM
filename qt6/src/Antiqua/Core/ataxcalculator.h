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
  double p_origin;
  int p_vat_type;
  AntiquaCRM::SalesTax p_vat_mode;

public:
  /**
   * @param price - money
   */
  explicit ATaxCalculator(double price, int vat_type);

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
   * @brief Set Current VAT Mode
   * @param mode
   */
  void setBillingMode(AntiquaCRM::SalesTax mode);

  /**
   * @brief Current VAT Billing mode
   * @sa AntiquaCRM::SalesTax
   */
  AntiquaCRM::SalesTax getBillingMode() const;

  /**
   * @brief Current VAT
   *
   * Returning current VAT-Type.
   * Referenced by PostgreSQL Table \b "article_orders" and his columns:
   * @li "a_type" - ArticleType
   * @li "a_tax"  - Tax Value Category
   *
   * @code
   *  pgsql=> \dt+ article_orders
   * @endcode
   *
   * Defined in Table Column: a_tax
   * @code
   *  0 = Normal VAT
   *  1 = Reduced VAT
   *  2 = Without VAT
   * @endcode
   * @sa AntiquaCRM::ArticleType
   *
   */
  int vatType() const;

  /**
   * @brief net price without any VAT calculation
   */
  double netprice() const;

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
  static const QString
  money(double value,
        QLocale::CurrencySymbolFormat format = QLocale::CurrencySymbol);
};

};     // namespace AntiquaCRM
#endif // ANTIQUACRM_ATAXCALCULATOR_H
