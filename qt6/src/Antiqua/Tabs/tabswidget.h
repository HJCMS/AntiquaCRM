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
#include <QScopedPointer>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {

class TabsIndex;
class TabsBar;

/**
 * @class TabsWidget
 * @brief Primary Interfaces TabWidget for Tab Plugins.
 *
 * Class member to handle AntiquaCRM::TabIndex childs.
 *
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsWidget final : public QTabWidget {
  Q_OBJECT

private:
  /**
   * @brief Temporary pointer to current removed class
   * @sa removeIndex
   */
  mutable AntiquaCRM::TabsIndex *m_tmp = nullptr;

  /**
   * @brief Read properties after insert and set changes.
   * @param index
   * @sa AntiquaCRM::TabsBar::setTabCloseable
   *
   * This virtual handler is called after a new tab was added or inserted at
   * position index.
   *
   * Searching for AntiquaCRM::TabsIndex and read closable properties.
   * When Tab removed the member class will copied to \b m_tmp and destroyed in
   * tabRemoved.
   */
  void tabInserted(int index) override;

  /**
   * @brief This virtual handler is called after a tab was removed.
   * @param index
   * @sa removeIndex
   *
   * This handler destroying the removed AntiquaCRM::TabsIndex member from
   * cache.
   */
  void tabRemoved(int index) override;

  /**
   * @brief close tab by index
   * @param index
   *
   * It searching for AntiquaCRM::TabsIndex and check for unsaved changes.
   * If all has been checked, remove it.
   *
   * Removes the tab at position index from this stack of widgets.
   * The page widget itself is not deleted.
   * @sa tabRemoved
   */
  bool removeIndex(int index);

private Q_SLOTS:
  /**
   * @brief Check for TabsIndex::ViewPage
   * @param index - current index
   *
   * If this Tab has property TabsIndex::ViewPage::MainView then calling method
   * onEnterChanged.
   */
  void setTabChanged(int index);

  /**
   * @brief Check and close tab with index
   * @param index
   *
   * If this Tab has not property TabsIndex::ViewPage::MainView or
   * AntiquaCRM::TabsBar::isTabCloseable responses true then calling \i
   * removeIndex Tab.
   */
  void setTabClosed(int index);

protected:
  /**
   * @brief Configuration member
   *
   * This member class is always initialized in constructor.
   */
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief TabBar member
   *
   * This member class is always initialized in constructor.
   */
  AntiquaCRM::TabsBar *m_tabBar;

  /**
   * @brief Sorting Tabs with this Listing after application start.
   *
   * Read from configuration:
   *  @li Group: window_behavior
   *  @li Section: sorting_tabs
   */
  QStringList p_sorting_tabs;

  /**
   * @brief Enable/Disable Mouse Wheel support on tab hover mouse
   *
   * Read from configuration:
   *  @li Group: window_behavior
   *  @li Section: mouse_wheel_support
   */
  bool p_wheel_support;

Q_SIGNALS:
  /**
   * @brief Notify status messages
   * @param msg - Notification string
   */
  void sendMessage(const QString &msg);

public Q_SLOTS:
  /**
   * @brief overload function for setCurrentIndex(int)
   * @param name = TabsIndex::tabIndexId()
   *
   * This slot search for TabsIndex::tabIndexId() and make it visible!
   * if name isEmpty then it will search for QObject::sender()->objectName()
   */
  void setCurrentTab(const QString &name = QString());

public:
  /**
   * @param parent - parent object
   */
  explicit TabsWidget(QWidget *parent = nullptr);

  /**
   * @brief fallback theme icon
   */
  static const QIcon defaultIcon();

  /**
   * @brief find tab by QObjectName
   * @param name = find with objectName
   */
  int indexByName(const QString &name) const;

  /**
   * @brief overload function for addTab
   * @param tab - object to insert
   *
   * Reading member properties and set AntiquaCRM::TabsBar::setTabCloseable and
   * calling QTabWidget::addTab and emit signal onEnterChanged.
   */
  int registerTab(AntiquaCRM::TabsIndex *tab);

  /**
   * @brief Sorting tabs to configuration default.
   */
  void sortTabs();

  /**
   * @brief overlaod function for QTabWidget::widget with object_cast
   * @param index
   */
  AntiquaCRM::TabsIndex *tabIndex(int index) const;

  /**
   * @brief overlaod function for tabWithIndex(int)
   * @param name - objectName
   */
  AntiquaCRM::TabsIndex *tabIndex(const QString &name) const;

  /**
   * @brief unload all loaded tab
   */
  bool unloadTabs();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABSWIDGET_H
