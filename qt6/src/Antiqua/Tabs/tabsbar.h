// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_TABSBAR_H
#define ANTIQUACRM_TABS_TABSBAR_H

#include <AGlobal>
#include <QObject>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsBar
 * @brief TabBar class for TabsInterface
 *
 * Default Properties for this class are Movable, Expanding and Tabs Closable.
 *
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsBar final : public QTabBar {
  Q_OBJECT

private:
  /**
   * @brief Wheel support status
   * @sa wheelEvent
   */
  bool p_wheel_support = false;

  /**
   * @brief Setting some defaults when a tab has been add.
   * @param index
   */
  void tabInserted(int index) override;

  /**
   * @brief Prevent wheel events, when \i p_wheel_support is disabled.
   * @param event
   */
  void wheelEvent(QWheelEvent *event) override;

public:
  /**
   * @param parent      - parent widget
   * @param wheelEvents - enable wheel event
   */
  explicit TabsBar(QWidget *parent = nullptr, bool wheelEvents = false);

  /**
   * @brief Modify properties
   * @param index
   * @param closeable
   *
   * Setting \b setTabData and \b setTabButton if needed.
   */
  void setTabCloseable(int index, bool closeable = false);

  /**
   * @brief Checking if Indexed Tab is closeable.
   * @param index
   */
  bool isTabCloseable(int index);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABSBAR_H
