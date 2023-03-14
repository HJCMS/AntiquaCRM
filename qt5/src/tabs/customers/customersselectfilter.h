// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSSELECTFILTER_H
#define ANTIQUACRM_CUSTOMERSSELECTFILTER_H

#include <QComboBox>
#include <QJsonObject>

/**
 * @brief Customers Filter selecter
 * @ingroup _customers
 */
class CustomersSelectFilter final : public QComboBox {
  Q_OBJECT

public:
  enum Filter {
    Customer = 0x1,   /**< @brief Kundensuche */
    CustomerId = 0x2, /**< @brief ID Suche */
    Company = 0x4     /**< @brief Unternehmens Suche */
  };
  Q_DECLARE_FLAGS(Filters, Filter)

  explicit CustomersSelectFilter(QWidget *parent = nullptr);

  CustomersSelectFilter::Filter currentFilter();

  /**
   * @brief Nehme aktuellen Suchfilter
   * @code
   *  QJsonObject(
   *    "search" => QJSonValue().toString()
   *    "fields" => QJSonValue().toString(),
   *  );
   * @endcode
   */
  const QJsonObject getFilter(CustomersSelectFilter::Filter filter);
};

Q_DECLARE_METATYPE(CustomersSelectFilter::Filter)

#endif // ANTIQUACRM_CUSTOMERSSELECTFILTER_H
