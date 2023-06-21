// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_INDEX_H
#define ANTIQUACRM_TABS_INDEX_H

#include <AGlobal>
#include <QEvent>
#include <QObject>
#include <QShortcut>
#include <QStackedWidget>

namespace AntiquaCRM {

/**
 * @class TabsIndex
 * @brief Primary Tab Inventory class
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsIndex : public QStackedWidget {
  Q_OBJECT
  Q_PROPERTY(bool closable READ isClosable WRITE setClosable NOTIFY
                 sendClosableChanged)

private:
  /**
   *  @brief Uniq Tab Index Name
   */
  const QString tabIndex;

  /**
   * @brief Searchline edit (Ctrl+Shift+S)!
   */
  QShortcut *m_focusSearch;

  /**
   * @brief Filter focus (Ctrl+Shift+F)!
   */
  QShortcut *m_focusFilter;

  /**
   * @brief Create new Entry Button (Ctrl+Shift+N)!
   */
  QShortcut *m_createEntry;

  /**
   * @brief registering all defined keyboard shortcuts
   */
  void addShortCutsAndSignals();

private Q_SLOTS:
  /**
   * @brief only emit sendSetSearchFocus when currentView() = MainView
   */
  void prepareShortCutSearchFocus();

  /**
   * @brief only emit sendSetSearchFilter when currentView() = MainView
   */
  void prepareShortCutSearchFilter();

  /**
   * @brief only call createNewEntry when currentView() = MainView
   */
  void prepareShortCutNewEntry();

protected:
  /**
   * @brief Is this Tab already loaded and initialed?
   *
   * This property is used to verify if this Widget is already loaded or not.
   *
   * The primary goal is to prevent multiple functional initial calls.
   *
   * @note This have to be changed in virtual slot @ref onEnterChanged.
   */
  bool initialed = false;

  /**
   * @brief is this tab closeable?
   *
   * This property must set in Constructor and can not changed afterwards.
   */
  bool closable = false;

  /**
   * @brief Register signals, editor reports unsaved records!
   * @note Must be installed with obj->installEventFilter(this)!
   */
  virtual bool eventFilter(QObject *, QEvent *) override final;

  /**
   * @brief Set it Closable.
   *
   * WRITE Property from @ref closable
   */
  void setClosable(bool b = false);

protected Q_SLOTS:
  /**
   * @brief Copy String to System Clipboard
   * @param data - Clipboard data
   *
   * Call QApplication::clipboard
   */
  void copyToClipboard(const QString &data);

  /**
   * @brief Send a message to the internal socket.
   *
   * Create Socket call @ref AntiquaCRM::ATransmitter::pushStatusBarMessage
   */
  void sendStatusMessage(const QString &message);

  /**
   * @brief Send a operation to the internal socket.
   *
   * Create Socket call @ref AntiquaCRM::ATransmitter::pushOperation
   */
  void sendSocketOperation(const QJsonObject &obj);

  /**
   * @brief Customized PopUp Warning
   * @param title   - PopUp title
   * @param message - Body
   *
   * Opens a predefined QMessageBox::warning dialog with title and body.
   */
  void openWarningPopUp(const QString &title, const QString &message);

  /**
   * @brief Open PopUp Warning - Editor is in edit mode!
   *
   * This Slot is reserved to do prepare PopUp Message if Editor is in use or
   * unsaved changes exists.
   */
  virtual void popupWarningTabInEditMode() = 0;

  /**
   * @brief Call standard table query
   *
   * This Slot is reserved to do prepare default Table views.
   */
  virtual void setDefaultTableView() = 0;

Q_SIGNALS:
  /**
   * @brief report Window modified status
   *
   * Propagate child set Window Modified to parent.
   */
  void sendModifiedStatus(bool);

  /**
   * @brief Send Tab closeable changed
   *
   * NOTIFY Property from @ref closable
   */
  void sendClosableChanged();

  /**
   * @brief If Shortcut m_focusSearch was triggered.
   */
  void sendSetSearchFocus();

  /**
   * @brief If Shortcut from m_focusFilter was triggered.
   */
  void sendSetSearchFilter();

  /**
   * @brief Article Id
   *
   * Send selected ArticleId
   */
  void sendArticleId(qint64 id);

public Q_SLOTS:
  /**
   * @brief open start page
   *
   * Reserved to set default page from this QStackedWidget.
   */
  virtual void openStartPage() = 0;

  /**
   * @brief prepare SQL Queries
   *
   * Reserved to prepare/check SQL Queries
   */
  virtual void createSearchQuery(const QString &query = QString()) = 0;

  /**
   * @brief create a new entry in this section
   *
   * Reserved to create new Entry.
   */
  virtual void createNewEntry() = 0;

  /**
   * @brief open Entry with Article ID
   *
   * Reserved to open a SerialId based Entry.
   */
  virtual void openEntry(qint64 id) = 0;

  /**
   * @brief caller for tab has changed to visible
   *
   * This Slot is reserved to do some  initialisation on first start.
   *
   * For example - read configuration settings or call first database queries on
   * start.
   *
   * See also @ref initialed
   * @code
   * void MyTabIndexClass::onEnterChanged() {
   *  if (initialed)
   *    return; // already initialed
   *
   *  // code example for first initial
   *  setCurrentIndex(0);
   *  setDefaultTableView();
   *  initialed = true;
   * }
   * @endcode
   */
  virtual void onEnterChanged() = 0;

public:
  enum ViewPage {
    MainView = 0,   /**< @brief index page Index:0 */
    EditorView = 1, /**< @brief editor page Index:1 */
    CustomView = 2  /**< @brief additional page index:2+ */
  };

  /**
   * @param index - Uniq tab Index Name
   * @param parent - parent object
   */
  explicit TabsIndex(const char *index, QWidget *parent = nullptr);

  /**
   * @brief Fixed Title for Window and Message boxes
   */
  virtual const QString getTitle() const = 0;

  /**
   * @brief Which Stacked index is currently in use?
   */
  TabsIndex::ViewPage currentView();

  /**
   * @brief Create Custom Entries
   *
   * Reserved for customized operation calls.
   *
   * @return Accepted or Rejected
   */
  virtual bool customAction(const QJsonObject &obj) = 0;

  /**
   * @brief Uniq Tab Index Identifier
   * @return configured Index in TabWidget.
   */
  const QString tabIndexId() const;

  /**
   * @brief Is this tab closable or not?
   * @return Property from @ref closable
   */
  bool isClosable();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_INDEX_H
