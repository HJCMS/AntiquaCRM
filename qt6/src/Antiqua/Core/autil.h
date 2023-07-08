// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_AUTIL_H
#define ANTIQUACRM_AUTIL_H

#include <AGlobal>
#include <QChar>
#include <QRegularExpression>
#include <QString>
#include <QTime>

namespace AntiquaCRM {

/**
 * @class AUtil
 * @brief Antiqua Utilities
 *
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
   * @param str - string
   * @warning Line Breaks will removed!
   */
  static const QString trim(const QString &str);

  /**
   * @brief uppercase first letter
   * @param str - string
   */
  static const QString ucFirst(const QString &str);

  /**
   * @brief Simple Regular Expression for eMail
   * @code
   *  ^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{2,})\\.([a-z]{2,6})$
   * @endcode
   */
  static const QRegularExpression emailRegExp();

  /**
   * @brief Check eMail Address
   * @param mail - email
   */
  static bool checkMail(const QString &mail);

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
   * @param phone - phone number
   */
  static bool checkPhone(const QString &phone);

  /**
   * @brief Regular expression to match keywords with UTF8 characters.
   * @code
   *   '^([\\w\\d]+)$/u'
   * @endcode
   */
  static const QRegularExpression keywordRegExp();

  /**
   * @brief Convert Number with leading zeros to string.
   * @param number - input
   * @param length - fill length
   */
  static const QString zerofill(qint64 number, int length = 7);

  static const QString md5sum(const QString &data);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AUTIL_H
