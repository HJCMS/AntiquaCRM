// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_MENU_H
#define ANTIQUACRM_TABS_MENU_H

#include <AGlobal>
#include <QIcon>
#include <QList>
#include <QMenu>
#include <QMenuBar>
#include <QSignalMapper>

namespace AntiquaCRM {

class TabsIndex;

/**
 * @class TabsMenu
 * @brief Menu for loaded Tabs
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsMenu final : public QMenu {
  Q_OBJECT

private:
  QSignalMapper *m_mapper;

Q_SIGNALS:
  void sendOpenTab(const QString &);

public Q_SLOTS:
  void addAction(AntiquaCRM::TabsIndex *);

public:
  explicit TabsMenu(QMenuBar *parent);
  virtual ~TabsMenu();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_MENU_H
