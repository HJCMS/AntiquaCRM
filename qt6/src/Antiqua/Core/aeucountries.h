// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_AEUCOUNTRIES_H
#define ANTIQUACRM_AEUCOUNTRIES_H

#include <AGlobal>
#include <QMap>
#include <QString>
#include <QObject>

namespace AntiquaCRM {

/**
 * @class AEUCountries
 * @brief European Countries data map
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY AEUCountries final : private QObject,
                                              public QMap<QString, QString> {
  Q_OBJECT

public:
  explicit AEUCountries(QObject *parent = nullptr);

  /**
   * @brief get bcp47Name with translated Country!
   * @param country
   * @return bcp47Name
   */
  const QString bcp47Name(const QString &country);

  /**
   * @brief get Country with bcp47Name!
   * @param bcp47
   * @return Translated Country name
   */
  const QString country(const QString &bcp47);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_AEUCOUNTRIES_H
