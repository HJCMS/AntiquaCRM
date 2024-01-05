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

class TabsStatusProgress;

class ANTIQUACRM_LIBRARY TabsStatusBar : public QFrame {
  Q_OBJECT
  Q_PROPERTY(QString filterName READ getFilterName WRITE setFilterName NOTIFY
                 sendFilterNameChanged);
  Q_PROPERTY(QString stockName READ getStockName WRITE setStockName NOTIFY
                 sendStockNameChanged);

public:
  enum History {
    Today = 1,
    Yesterday = 2,
    LastWeek = 3,
    LastMonth = 4,
    ThisWeek = 5,
    ThisMonth = 6,
    ThisYear = 7,
    NOIMAGE = 8,
    Customized = 9
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
   * @brief Info Widget
   */
  TabsStatusProgress *m_status;

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

  /**
   * @brief SQL field names used by @ref setHistoryAction
   * @{
   */
  QString filterName;
  QString stockName;
  QString imgFilter = QString("AND im_id IS NULL");
  /**
   * @}
   */

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
   * @brief Set Fieldname for filter by TIMESTAMP
   * @param name - Fieldname for filtering date times
   */
  void setFilterName(const QString &name);

  /**
   * @brief Set Fieldname for filter by stock
   * @param name - Fieldname for filtering with crowd
   */
  void setStockName(const QString &name);

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
   * @brief History actions slot
   * @note Some hints before use it
   *  @li Because of SignalMapper the parameter is an integer!
   *  @li This slot requires some filter name settings before you can use it.
   *  @li The signal processing must enabled in subclasses.
   *
   * @sa setHistoryActionMenu, setFilterName, setStockName
   *
   * Alternatively you can override this method to create your own actions.
   */
  virtual void setHistoryAction(int);

Q_SIGNALS:
  /**
   * @brief This signal will emitted when filterName has changed
   */
  void sendFilterNameChanged(const QString &);

  /**
   * @brief This signal will emitted when stockName has changed
   */
  void sendStockNameChanged(const QString &);

  /**
   * @brief Reserved signal for History queries
   */
  void sendHistoryQuery(const QString &);

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
   * @brief Set message query has initiated
   */
  void startProgress();
  void finalizeProgress();

  /**
   * @brief current field name for date filter selection
   */
  const QString getFilterName();

  /**
   * @brief current field name for crowd filtering
   */
  const QString getStockName();

  /**
   * @brief Status bar message
   * @param message
   * @param timeout
   */
  void showMessage(const QString &message, int timeout = 0);

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
