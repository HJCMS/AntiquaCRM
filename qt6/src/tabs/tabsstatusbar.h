// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_TABS_STATUSBAR_H
#define ANTIQUA_TABS_STATUSBAR_H

#include <QFrame>
#include <QIcon>
#include <QLayout>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QStatusBar>

class TabsStatusBar : public QStatusBar {
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
   * @brief Erstellen Knopf abfragen
   */
  virtual bool isCreateButtonEnabled() = 0;

  /**
   * @brief Menüeinträge mit den TabStatusBar::History werten!
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
  QPushButton *createButton(const QString &title = QString());

  /**
   * @brief optinal - history button
   */
  QPushButton *historyButton(const QString &title = QString());

  /**
   * @brief optinal - default view button
   */
  QPushButton *defaultViewButton(const QString &title = QString());

  /**
   * @brief Erstellt aus @ref historyItems() Menüeinträge
   * @note  Einträge lösen den SLOT @ref setHistoryAction(int) aus!
   */
  void setHistoryActionMenu(QPushButton *parent);

  /**
   * @brief Helferlein für Icon zuweisung
   */
  static const QIcon getIcon(const QString &name);

protected Q_SLOTS:
  /**
   * @brief Auswahl von Menüeinträgen verarbeiten!
   * Es wird "TabStatusBar::History" übergeben.
   * @note Wegen SignalMapper ist der Parameter ein INT!
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

Q_DECLARE_METATYPE(TabsStatusBar::History)

#endif // ANTIQUA_TABS_STATUSBAR_H
