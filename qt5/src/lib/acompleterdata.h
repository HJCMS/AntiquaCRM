// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ACOMPLETERDATA_H
#define ANTIQUACRM_ACOMPLETERDATA_H

#include <AGlobal>
#include <QJsonObject>
#include <QString>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief The ACompleterData class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ACompleterData final {
private:
  const QString p_basename;
  const QJsonObject getTreeObject() const;

public:
  explicit ACompleterData(const QString &basename);
  const QStringList completition(const QStringList &keys);
  const QStringList completition(const QString &section);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ACOMPLETERDATA_H
