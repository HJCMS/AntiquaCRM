// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_MENU_H
#define ANTIQUACRM_TABS_MENU_H

#include <AntiquaWidgets>
#include <QAction>
#include <QIcon>
#include <QJsonObject>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QSignalMapper>

namespace AntiquaCRM {

/**
 * @class TabsMenu
 * @brief Menu for loaded Tabs
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsMenu final : public AMenu {
  Q_OBJECT

private:
  void addActions() override;

public Q_SLOTS:
  /**
   * @brief Create insert Action entry
   * @param jo - json object
   * @sa @ref TabsInterface::menuEntry for QJsonObject structure describtion.
   */
  void addAction(const QJsonObject &);

public:
  /**
   * @param parent - parent object
   */
  explicit TabsMenu(QMenuBar *parent);

  /**
   * @brief Action entry exist in Menu?
   * @param id - identifier and mapper:id
   */
  bool exists(const QString &id);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_MENU_H
