// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
   *  @brief Uniq Tab Index and Object Name
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
   * @brief indicates if this Tab already loaded and initialized.
   *
   * The primary goal is to prevent multiple functional initial calls.
   *
   * @note This have to be changed in virtual slot @ref onEnterChanged.
   */
  bool initialed = false;

  /**
   * @brief indicates if this Tab is closable or not.
   *
   * This property must set in Constructor and can not changed afterwards.
   */
  bool closable = false;

  /**
   * @brief Set it Closable.
   *
   * Q_PROPERTY WRITE - for @ref closable
   */
  void setClosable(bool b = false);

  /**
   * @brief Register signals, editor reports unsaved records!
   *
   * This event filter search for isWindowModified in AntiquaCRM::TabsEditor
   * classes and have installed manual in subclass from AntiquaCRM::TabsIndex.
   *
   * @code
   * m_scrollArea = new QScrollArea(this);
   * m_scrollArea->setWidgetResizable(true);
   * m_editor = new PluginTabEditor(m_scrollArea);
   * m_editor->installEventFilter(this);
   * m_scrollArea->setWidget(m_editor);
   * insertWidget(1, m_scrollArea);
   * @endcode
   */
  virtual bool eventFilter(QObject *, QEvent *) override final;

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
   * @param obj - socket operation
   *
   * Creates a socket call to @ref AntiquaCRM::ATransmitter::pushOperation
   *
   * About operation definition @see AntiquaCRM::ATransmitter class.
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
   * @param title WindowTitle
   */
  void openWarningPopUpPageIndex(const QString &title);

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
   * Q_PROPERTY NOTIFY - for @ref closable
   */
  void sendClosableChanged();

  /**
   * @brief If Shortcut (Ctrl+Shift+S) was triggered.
   */
  void sendSetSearchFocus();

  /**
   * @brief If Shortcut (Ctrl+Shift+F) was triggered.
   */
  void sendSetSearchFilter();

  /**
   * @brief Article Id
   *
   * Predefined signal for Article Id selection.
   * Mostly needed by Subclasses and not used in this main class abstraction.
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
   * e.g. Read configuration settings to call first database queries on start.
   *
   * See also @ref initialed
   * @code
   * void MyTabIndexClass::onEnterChanged() {
   *  if (initialed)
   *    return; // already initialed
   *
   *  setCurrentIndex(ViewPage::MainView);
   *  setDefaultTableView();
   *  initialed = true;
   * }
   * @endcode
   */
  virtual void onEnterChanged() = 0;

public:
  /**
   * @brief Enumeration to find the right Stacked page.
   * @sa currentPage
   *
   * Used to find the right Page on load and exit.
   * The Application will reject close functions, if current index is in
   * ViewPage::EditorView mode.
   *
   * Only "MainView" and "EditorView" will called from the Application, other
   * Pages not known by the System and only useable from current tab plugin.
   */
  enum ViewPage {
    MainView = 0,   /**< @brief Main page Normally Index:0 */
    EditorView = 1, /**< @brief Editor page Normally Index:1 */
    CustomView = 2  /**< @brief Additional page Index:2 */
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
   * @brief Create Custom Action for Open/Edit Import.
   *
   * Reserved to create customized operation socket calls.
   *
   * @return Accepted or Rejected
   */
  virtual bool customAction(const QJsonObject &obj) = 0;

  /**
   * @brief List of Acceptable Custom Actions.
   */
  virtual const QStringList acceptsCustomActions() const = 0;

  /**
   * @brief Uniq Tab Index Identifier
   * @note The Index Identifier is equal to "objectName"!
   */
  const QString tabIndexId() const;

  /**
   * @brief Is this tab closable or not?
   *
   * Q_PROPERTY READ - for @ref closable
   */
  bool isClosable();

  /**
   * @brief Which Page is currently in use?
   *
   * Only Enumeration will returned. If you have more pages write your own
   * operation.
   */
  TabsIndex::ViewPage currentPage();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_INDEX_H
