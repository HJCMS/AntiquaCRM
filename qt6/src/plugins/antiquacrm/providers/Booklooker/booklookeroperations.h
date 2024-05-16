// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_OPERATIONS_H
#define BOOKLOOKER_PLUGIN_OPERATIONS_H

#include <AGlobal>
#include <QEvent>
#include <QFrame>
#include <QObject>
#include <QWidget>

/**
 * @class BookLookerOperations
 * @brief BookLooker Radio buttons operations
 *
 * @ingroup BookLooker
 */
class ANTIQUACRM_LIBRARY BookLookerOperations final : public QFrame {
  Q_OBJECT

Q_SIGNALS:
  /**
   * @brief Send Window modification status
   */
  void sendSelectionModified(bool);

public:
  /**
   * @param parent - parent object
   */
  explicit BookLookerOperations(QWidget *parent = nullptr);
};

#endif // BOOKLOOKER_PLUGIN_OPERATIONS_H
