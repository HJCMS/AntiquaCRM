// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_AUTIL_H
#define ANTIQUACRM_AUTIL_H

#include <AGlobal>
#include <QChar>
#include <QLocale>
#include <QRegExp>
#include <QString>
#include <QTime>

namespace AntiquaCRM {

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
  static const QRegExp emailRegExp();

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
  static const QRegExp phoneRegExp();

  /**
   * @brief Check Mobile/Phone Addresses
   */
  static bool checkPhone(const QString &);

  /**
   * @brief Convert from System Codec to ISO-8859-1
   */
  static const QString toISO88591(const QString &);

  /**
   * @brief Convert Number to AntiquaCRM with leading zeros to String
   */
  static const QString zerofill(qint64 number, int length = 7);

  /**
   * @brief Convert double to money value
   * @param value
   */
  static const QString toMoney(double value,
    QLocale::CurrencySymbolFormat format = QLocale::CurrencySymbol);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AUTIL_H
