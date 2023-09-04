// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ACTIONSBAR_H
#define ABEBOOKS_PLUGIN_ACTIONSBAR_H

#include <AGlobal>
#include <QAction>
#include <QToolBar>
#include <QWidget>

/**
 * @class AbeBooksActionsBar
 * @brief AbeBooks Navigation bar
 *
 * @ingroup AbeBooks
 */
class ANTIQUACRM_LIBRARY AbeBooksActionsBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_left;
  QAction *ac_right;

private Q_SLOTS:
  void goToPage();

Q_SIGNALS:
  void goPrevious();
  void goNext();

public:
  explicit AbeBooksActionsBar(QWidget *parent = nullptr);
};

#endif // ABEBOOKS_PLUGIN_ACTIONSBAR_H
