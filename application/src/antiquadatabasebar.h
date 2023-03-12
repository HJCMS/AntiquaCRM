// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_DATABASEBAR_H
#define ANTIQUACRM_DATABASEBAR_H

#include <QAction>
#include <QObject>
#include <QToolBar>
#include <QWidget>

/**
 * @ingroup ui
 * @brief Database connection status bar
 */
class AntiquaDatabaseBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_status;

private Q_SLOTS:
  void databaseInfoDialog();

public Q_SLOTS:
  /**
   * @brief set icon status
   */
  void setStatus(bool);

public:
  explicit AntiquaDatabaseBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_DATABASEBAR_H
