// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_MENU_H
#define ANTIQUACRM_STATISTICS_MENU_H

#include <AntiquaWidgets>
#include <QAction>

/**
 * @class StatisticsMenu
 * @brief Menu to open Statistics
 * @ingroup Utilities
 */
class StatisticsMenu final : public AntiquaCRM::AMenu {
  Q_OBJECT

private:
  void addActions() override;

public:
  /**
   * @param parent - parent object
   */
  explicit StatisticsMenu(QMenuBar *parent);
};

#endif // ANTIQUACRM_STATISTICS_MENU_H
