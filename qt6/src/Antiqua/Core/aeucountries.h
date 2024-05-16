// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_AEUCOUNTRIES_H
#define ANTIQUACRM_AEUCOUNTRIES_H

#include <AGlobal>
#include <QList>
#include <QLocale>
#include <QMap>
#include <QString>

namespace AntiquaCRM {

/**
 * @class AEUCountries
 * @brief European Countries data map
 *
 * @code
 *  QMap<
 *      KEY,  // ISO 3166-1 alpha-2
 *      NAME  // Native Country Name in QLocale::c()
 *      >
 * @endcode
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY AEUCountries final : private QLocale,
                                              public QMap<QString, QString> {

public:
  /**
   * @param locale - set QLocale for this query
   */
  explicit AEUCountries(const QLocale &locale = QLocale::c());

  /**
   * @brief get ISO 3166-1 alpha-2 with Country!
   * @param country
   * @return ISO 3166-1 alpha-2
   */
  const QString isoCode(const QString &country);

  /**
   * @brief get country with ISO 3166-1 alpha-2!
   * @param iso - ISO 3166-1 alpha-2
   * @return country name in „c“
   */
  const QString countryName(const QString &iso);

  /**
   * @brief Locale list for current European Union members
   */
  static const QList<QLocale::Country> europeanUnion();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_AEUCOUNTRIES_H
