// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_MAINWINDOW_H
#define ANTIQUA_UI_MAINWINDOW_H

#include <AntiquaTabs>
#include <QList>
#include <QMainWindow>
#include <QObject>

class MenuBar;
class StatusBar;

/**
 * @class MainWindow
 * @brief AntiquaCRM Mainwindow
 *
 * @ingroup Application
 */
class MainWindow final : public QMainWindow {
  Q_OBJECT

private:
  /**
   * @brief Configuration will initialed in openWindow()
   */
  AntiquaCRM::ASettings *config = nullptr;

  /**
   * @brief Window Menu bar
   */
  MenuBar *m_menuBar;

  /**
   * @brief Tabs Menu, entries filled by loadTabInterfaces
   */
  AntiquaCRM::TabsMenu *m_tabsMenu;

  /**
   * @brief Tabs Interfaces list
   */
  QList<AntiquaCRM::TabsInterface *> tabInterfaces;

  /**
   * @brief Central Tab widget for all operations!
   */
  AntiquaCRM::TabsWidget *m_tabWidget;

  /**
   * @brief Window Status bar
   */
  StatusBar *m_statusBar;

  /**
   * @brief Socket receiver
   */
  AntiquaCRM::AReceiver *m_rx;

  /**
   * @brief Initial and start local Socket server.
   */
  bool createSocketListener();

  /**
   * @brief Loading available AntiquaCRM::TabsInterface’s
   *
   * Use AntiquaCRM::TabsLoader to find all loadable Interfaces.
   * Create a Menu entry and fill @ref p_tabsIndexList container.
   */
  bool loadTabInterfaces();

  /**
   * @brief Search Tabs view action with Id.
   * @param id - object name
   *
   * If Action exists search for existing Tab and view it.
   *
   * If AntiquaCRM::TabsIndex not present in Tab Widget,
   * AntiquaCRM::TabsWidget::registerTab is called.
   */
  bool tabViewAction(const QString &id);

private Q_SLOTS:
  /**
   * @brief Set property, window or children has unsaved changes.
   *
   * Forwarding to MainWindow::setWindowModified(bool)
   *
   * Caused by:
   *  @li AntiquaCRM::TabsIndex::sendModifiedStatus
   */
  void setChanges(bool);

  /**
   * @brief Slot to View/Open tabs
   * @param name - identifier
   */
  void setViewTab(const QString &name);

  /**
   * @brief Set/Send Tab operations
   * @param name - tab target object name
   * @param data - operation data
   *
   * This slot is called by Socket server.
   */
  void setAction(const QString &name, const QJsonObject &data);

Q_SIGNALS:
  void sendApplicationQuit();

public Q_SLOTS:
  /**
   * @brief toggle window visibility
   */
  Q_INVOKABLE void setToggleWindow();

  /**
   * @brief put window in fullscreen
   */
  Q_INVOKABLE void setToggleFullScreen();

public:
  /**
   * @param parent - parent object
   */
  explicit MainWindow(QWidget *parent = nullptr);

  /**
   * @brief Primary Window initialisation
   *
   * Important function to open the Main Window and read Settings.
   * All dependent classes will initialized in this function!
   */
  bool openWindow();

  /**
   * @brief Safely close Widgets before unload all Members.
   */
  bool closeWindow();

  /**
   * @brief Mainwindow destructor.
   */
  virtual ~MainWindow();
};

#endif // ANTIQUA_UI_MAINWINDOW_H
