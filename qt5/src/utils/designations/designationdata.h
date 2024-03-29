// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DESIGNATIONS_DATA_H
#define DESIGNATIONS_DATA_H

#include <QMetaType>
#include <QString>

/**
 * @brief The DesignationData class
 * @ingroup _designations
 */
struct DesignationData {
  int id = -1;
  QString keyword;
  QString description;
};

Q_DECLARE_METATYPE(DesignationData);

#endif // DESIGNATIONS_DATA_H
