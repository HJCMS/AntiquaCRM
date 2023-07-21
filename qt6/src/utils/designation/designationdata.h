// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DESIGNATIONSDATA_H
#define ANTIQUACRM_UTILS_DESIGNATIONSDATA_H

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

#endif // ANTIQUACRM_UTILS_DESIGNATIONSDATA_H
