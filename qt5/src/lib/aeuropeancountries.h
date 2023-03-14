// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_EUROPEAN_COUNTRIES_H
#define ANTIQUACRM_EUROPEAN_COUNTRIES_H

#include <AGlobal>
#include <QHash>
#include <QString>

namespace AntiquaCRM {
/**
 * @brief European Countries Container
 * EU Countries initialed with ISO 3166-1 Alpha-2 Code.
 */
class ANTIQUACRM_LIBRARY AEuropeanCountries final
    : public QHash<QString, QString> {
public:
  explicit AEuropeanCountries();
};
}; // namespace AntiquaCRM

#endif // AEUROPEANCOUNTRIES_H
