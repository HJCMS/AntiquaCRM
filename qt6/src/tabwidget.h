// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_TABWIDGET_H
#define ANTIQUA_UI_TABWIDGET_H

#include <ASettings>
#include <AntiquaTabs>
#include <QMainWindow>
#include <QMap>
#include <QTabWidget>

class TabWidget final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::AReceiver *m_server;
  TabsBar *m_tabBar;

public:
  explicit TabWidget(QMainWindow *parent = nullptr);

  /**
   * @brief TimerEvents und Sockets sauber runterfahren!
   */
  virtual ~TabWidget();

  /**
   * @brief Liste der Tabs
   * @note Wird von Menüklassen verwendet!
   */
  static const QMap<QString, QString> availableTabs();

  /**
   * @brief Alle Registerkarten schließen!
   * Wenn eine Karte im Editormodus ist, wird das schließen verweigert! Und die
   * Methode bricht ab! Wahr wird nur bei Erfolgreichem schließen aller
   * Registerkarten zurück gegeben!
   */
  bool beforeCloseAllTabs();
};

#endif // ANTIQUA_UI_TABWIDGET_H
