// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_MENUBAR_H
#define ANTIQUA_UI_MENUBAR_H

#include <QIcon>
#include <QMenuBar>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {
class TabsMenu;
}

class MenuBar final : public QMenuBar {
  Q_OBJECT

private:
  QMenu *m_applMenu;
  QMenu *m_configMenu;
  QMenu *m_dialogMenu;
  QMenu *m_aboutMenu;

Q_SIGNALS:
  void sendApplicationQuit();

public Q_SLOTS:
  void openConfigDialog();
  void openDeliveryDialog();
  void openDesignationDialog();
  void openCompanyDialog();
  void openKeywordsDialog();
  void openStoragesDialog();
  void openPublisherDialog();

public:
  AntiquaCRM::TabsMenu *tabsMenu;
  explicit MenuBar(QWidget *parent = nullptr);
  static const QIcon tabIcon(const QString &name = QString("action-tab"));
};

#endif // ANTIQUA_UI_MENUBAR_H
