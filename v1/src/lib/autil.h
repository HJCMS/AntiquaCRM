// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_AUTIL_H
#define ANTIQUACRM_AUTIL_H

#include <AGlobal>
#include <QRegExp>
#include <QString>

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
  static const QRegExp mailPattern();

  /**
   * @brief Check eMail Address
   */
  static bool checkMail(const QString &);

  /**
   * @brief Regular Expression for Phone Numbers
   */
  static const QRegExp phonePattern();

  /**
   * @brief Check Mobile/Phone Addresses
   */
  static bool checkPhone(const QString &);

  /**
   * @brief Convert from System Codec to ISO-8859-1
   */
  static const QString toISO88591(const QString &);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_AUTIL_H
