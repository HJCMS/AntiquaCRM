// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_BAR_H
#define ANTIQUACRM_TABS_BAR_H

#include <AGlobal>
#include <QObject>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsBar
 * @brief Abstract TabBar class for TabsInterface
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsBar final : public QTabBar {
  Q_OBJECT

private:
  bool enableWheel;
  int index = -1;

protected:
  /**
   * @brief some defaults when a tab has been add
   * @param index
   */
  void tabInserted(int index) override;

  /**
   * @brief prevent wheel events, when enableWheel is true
   * @param event
   */
  void wheelEvent(QWheelEvent *event) override;

protected Q_SLOTS:
  /**
   * @brief tab changed with index
   * @param index
   */
  void tabIndexChanged(int);

  /**
   * @brief is this tabe closeable
   */
  void checkToClose();

Q_SIGNALS:
  /**
   * @brief tab has changed
   * @param index
   */
  void sendTabChanged(int index);

  /**
   * @brief tab has to be closed
   * @param index
   */
  void sendCloseTab(int index);

public:
  /**
   * @param parent      - parent widget
   * @param wheelEvents - enable wheel event
   */
  explicit TabsBar(QWidget *parent = nullptr, bool wheelEvents = false);

  /**
   * @brief Modify properties
   * @param index     - use index
   * @param closeable - is it closeable?
   */
  void setTabCloseable(int index, bool closeable = false);
  virtual ~TabsBar();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_BAR_H
