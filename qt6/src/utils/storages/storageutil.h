// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGEUTIL_H
#define ANTIQUACRM_UTILS_STORAGEUTIL_H

#include <QMetaType>
#include <QString>

/**
 * @brief The RowValues class
 * @ingroup _storage
 */
struct StorageItemData {
  int sl_id;
  QString sl_storage;
  QString sl_identifier;
  QString sl_location;
  qint64 sl_zvab_id;
  QString sl_zvab_name;
  qint64 sl_booklooker_id;
  QString sl_booklooker_name;
};

Q_DECLARE_METATYPE(StorageItemData);

#endif // ANTIQUACRM_UTILS_STORAGEUTIL_H
