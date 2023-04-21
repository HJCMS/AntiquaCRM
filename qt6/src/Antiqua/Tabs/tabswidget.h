// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_TABSWIDGET_H
#define ANTIQUACRM_TABS_TABSWIDGET_H

#include <AGlobal>
#include <ASettings>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class TabsBar;

class ANTIQUACRM_LIBRARY TabsWidget final : public QTabWidget {
  Q_OBJECT

private:
  void tabInserted(int) override;

private Q_SLOTS:
  void checkChanged(int);

protected:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::TabsBar *m_tabBar;

Q_SIGNALS:
  void sendMessage(const QString &);

public Q_SLOTS:
  void setViewTab();

  void setCurrentTab(const QString &id);

public:
  explicit TabsWidget(QWidget *parent = nullptr);

  int indexByName(const QString &);

  int registerTab(AntiquaCRM::TabsIndex *, const QString &title);

  static const QIcon defaultIcon();

  AntiquaCRM::TabsIndex *tabWithIndex(int);

  AntiquaCRM::TabsIndex *tabWithName(const QString &);

  bool unloadTabs();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABSWIDGET_H
