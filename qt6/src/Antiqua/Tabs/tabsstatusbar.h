// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_STATUSBAR_H
#define ANTIQUACRM_TABS_STATUSBAR_H

#include <AGlobal>
#include <QFrame>
#include <QIcon>
#include <QLayout>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QSignalMapper>
#include <QStatusBar>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY TabsStatusBar : public QStatusBar {
  Q_OBJECT

public:
  enum History {
    Today = 1,
    Yesterday = 2,
    LastWeek = 3,
    LastMonth = 4,
    ThisWeek = 5,
    ThisMonth = 6,
    ThisYear = 7,
    NOIMAGE = 8
  };
  explicit TabsStatusBar(QWidget *parent = nullptr);

  /**
   * Search and destroy - History Menues and SignalMapper - if exists!
   */
  virtual ~TabsStatusBar();

  /**
   * @brief Initial Create button
   */
  virtual bool isCreateButtonEnabled() = 0;

  /**
   * @brief Default TabStatusBar::History Menu entries.
   */
  static const QMap<TabsStatusBar::History, QString> historyItems();

private:
  /**
   * @brief Buttonsets frame
   */
  QFrame *m_frame;

  /**
   * @brief Buttons layout
   */
  QHBoxLayout *layout;

  /**
   * @brief Refresh Button
   */
  QPushButton *btn_refresh;

protected:
  /**
   * @brief Predifined Signal Mapper for History Menu
   */
  QSignalMapper *m_mapper = nullptr;

  /**
   * @brief Search only With Stock
   */
  bool SearchWithStock = false;

  /**
   * @brief add push button to permanent frame layout
   */
  void addButton(QPushButton *);

  /**
   * @brief optinal - create button
   */
  QPushButton *createButton(const QString &title = QString(),
                            const QString &tip = QString());

  /**
   * @brief optinal - history button
   */
  QPushButton *historyButton(const QString &title = QString());

  /**
   * @brief optinal - default view button
   */
  QPushButton *defaultViewButton(const QString &title = QString());

  /**
   * @brief Default icon for History menu entries.
   */
  const QIcon historyIcon() const;

  /**
   * @brief Create History action menu for History Button.
   *
   * In this function you can create customized History Action Menu.
   * There is also a predefined SignalMapper to create the @ref
   * setHistoryAction slot!
   */
  virtual void setHistoryActionMenu(QPushButton *parent) = 0;

protected Q_SLOTS:
  /**
   * @brief History actions Slot
   * @note Because of SignalMapper the parameter is an integer!
   * @sa setHistoryActionMenu
   */
  virtual void setHistoryAction(int) = 0;

Q_SIGNALS:
  /**
   * @brief Reserved signal for History queries
   */
  void sendHistoryQuery(const QString &query);

  /**
   * @brief Triggered by „Create Button“
   */
  void sendCreateEntry();

  /**
   * @brief Triggered by „Refresh Button“
   */
  void sendReloadView();

  /**
   * @brief Triggered by „Default View Button“
   */
  void sendDefaultView();

public Q_SLOTS:
  /**
   * @brief With „Stock“ search enabled or not!
   */
  void setStockEnabled(bool);

  /**
   * @brief Erstellen Knopf aktivieren/deaktivieren
   */
  virtual void setCreateButtonEnabled(bool) = 0;
};

} // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::TabsStatusBar::History)

#endif // ANTIQUACRM_TABS_STATUSBAR_H
