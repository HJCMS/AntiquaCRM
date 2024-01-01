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
#include <QUrl>

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
   * @brief eMail recipient pattern
   */
  static const QString recipientPattern();

  /**
   * @brief Domain pattern
   */
  static const QString domainPattern();

  /**
   * @brief Top Level Domain pattern
   */
  static const QString tldPattern();

  /**
   * @brief Full qualified domain name pattern
   */
  static const QString fqdnPattern();

  /**
   * @brief AntiquaCRM Socket Identifier
   */
  static const QString socketName();

  /**
   * @brief Simple Regular Expression for line breaks
   */
  static const QRegularExpression lineBreakRegExp();

  /**
   * @brief prepare and normalize single strings
   * @param str - string
   * @warning Line Breaks will removed!
   */
  static const QString trim(const QString &str);

  /**
   * @brief prepare and normalize multiline text body
   * @param str - string
   */
  static const QString trimBody(const QString &str);

  /**
   * @brief uppercase first letter
   * @param str - string
   */
  static const QString ucFirst(const QString &str);

  /**
   * @brief Simple Regular Expression for eMail
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
   *  "^(0[\\d]{1,3}\\s?[\\d]{2,4}[\\s?\\d]+)$"
   * @endcode
   */
  static const QRegularExpression phoneRegExp();

  /**
   * @brief Check Mobile/Phone Addresses
   * @param phone - phone number
   */
  static bool checkPhone(const QString &phone);

  /**
   * @brief Simple Url check
   * @param url - Url
   */
  static bool checkUrl(const QUrl &url);

  /**
   * @brief Regular expression to match keywords with UTF8 characters.
   * @code
   *   '^([\\w\\d]+)$/u'
   * @endcode
   */
  static const QRegularExpression keywordRegExp();

  /**
   * @short Simple Numeric validator
   * @code
   *  "^\\d+$"
   * @endcode
   */
  static const QRegularExpression numericRegExp();

  /**
   * @short Search in characters for none digits.
   * @code
   *  "\\D+"
   * @endcode
   */
  static const QRegularExpression nonDigitsRegExp();

  /**
   * @short AntiquaCRM find TABs and SPACEs
   * @code
   *  "[\\s\\t]+"
   * @endcode
   */
  static const QRegularExpression spaceRegExp();

  /**
   * @short AntiquaCRM Article number validation
   *
   * The item number system in Antiqua CRM consists of consecutive numbers.
   * When searching, these may also be entered separated by commas.
   * @code
   *  "^(\\d{1,9}[\\,]?)+$"
   * @endcode
   */
  static const QRegularExpression articleRegExp();

  /**
   * @brief Convert Number with leading zeros to string.
   * @param number - input
   * @param length - fill length
   */
  static const QString zerofill(qint64 number, int length = 7);

  /**
   * @brief MD5 sum from String
   * @param data
   */
  static const QString md5sum(const QString &data);

  /**
   * @brief Convert String to URL Query String with '+' delimiter for spaces!
   * @param query
   */
  static const QString strEncode(const QString &query);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AUTIL_H
