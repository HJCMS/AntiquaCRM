// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_TABWIDGET_H
#define ANTIQUACRM_TABS_TABWIDGET_H

#include <AGlobal>
#include <ASettings>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class TabsBar;

class ANTIQUACRM_LIBRARY TabWidget final : public QTabWidget {
  Q_OBJECT

private:
  void tabInserted(int) override;

protected:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::TabsBar *m_tabBar;

public Q_SLOTS:
  void setViewTab();

  void setCurrentTab(const QString &id);

public:
  explicit TabWidget(QWidget *parent = nullptr);

  int indexByName(const QString &);

  int registerTab(AntiquaCRM::TabsIndex *, const QString &title);

  static const QIcon defaultIcon();

  AntiquaCRM::TabsIndex *tabWithIndex(int);

  AntiquaCRM::TabsIndex *tabWithName(const QString &);

  /**
   * @brief Alle Registerkarten schließen!
   * Wenn eine Karte im Editormodus ist, wird das schließen verweigert! Und die
   * Methode bricht ab! Wahr wird nur bei Erfolgreichem schließen aller
   * Registerkarten zurück gegeben!
   */
  bool beforeCloseAllTabs();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABWIDGET_H
