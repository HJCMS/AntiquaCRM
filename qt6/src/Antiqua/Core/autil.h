// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_AUTIL_H
#define ANTIQUACRM_AUTIL_H

#include <AGlobal>
#include <QChar>
#include <QLocale>
#include <QRegularExpression>
#include <QString>
#include <QTime>

namespace AntiquaCRM {

/**
 * @brief Antiqua Utilities
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY AUtil final {
public:
  /**
   * @brief AntiquaCRM Socket Identifier
   */
  static const QString socketName();

  /**
   * @brief prepare and normalize single strings
   */
  static const QString trim(const QString &);

  /**
   * @brief uppercase first letter
   */
  static const QString ucFirst(const QString &);

  /**
   * @brief Regular Expression for eMail
   */
  static const QRegularExpression emailRegExp();

  /**
   * @brief Check eMail Address
   */
  static bool checkMail(const QString &);

  /**
   * @brief Simple Phone Regular expression
   * Containing "ITU-T E.123", "ITU-T E.164" and EPP definition.
   * @code
   *  (Country Code) (NPA Area Code) (Prefix) (Subscriber)
   * @endcode
   */
  static const QRegularExpression phoneRegExp();

  /**
   * @brief Check Mobile/Phone Addresses
   */
  static bool checkPhone(const QString &);

  /**
   * @brief Convert Number to AntiquaCRM with leading zeros to String
   */
  static const QString zerofill(qint64 number, int length = 7);

  /**
   * @brief Convert double to money value
   * @param value
   * @param format
   */
  static const QString toMoney(double value,
    QLocale::CurrencySymbolFormat format = QLocale::CurrencySymbol);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AUTIL_H
