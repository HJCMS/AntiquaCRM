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
   * @brief is this tab already loaded and initialed?
   */
  bool initialed = false;

  /**
   * @brief is this tab closeable?
   */
  bool closable = false;

  /**
   * @brief Register signals, editor reports unsaved records!
   * @note Must be installed with obj->installEventFilter(this)!
   */
  bool eventFilter(QObject *, QEvent *) override;

  /**
   * @brief Set it Closable.
   * @ref closable
   */
  void setClosable(bool b = false);

protected Q_SLOTS:
  /**
   * @brief Copy String to System Clipboard
   */
  void copyToClipboard(const QString &data);

  /**
   * @brief Send a message to the internal socket.
   */
  void sendStatusMessage(const QString &message);

  /**
   * @brief Send a operation to the internal socket.
   */
  void sendSocketOperation(const QJsonObject &obj);

  /**
   * @brief Open customized QMessageBox::warning
   */
  void openWarningPopUp(const QString &title, const QString &message);

  /**
   * @brief Open QMessageBox::warning - Editor is in edit mode!
   */
  virtual void popupWarningTabInEditMode() = 0;

  /**
   * @brief Standard Tabellenabfrage aufrufen
   */
  virtual void setDefaultTableView() = 0;

Q_SIGNALS:
  /**
   * @brief Speichernstände mitteilen
   */
  void sendModifiedStatus(bool);

  /**
   * @brief Send Tab closeable changed
   */
  void sendClosableChanged();

  /**
   * @brief signal from Shortcut m_focusSearch
   */
  void sendSetSearchFocus();

  /**
   * @brief signal from Shortcut m_focusFilter
   */
  void sendSetSearchFilter();

  /**
   * @brief Article Id
   */
  void sendArticleId(qint64 articleId);

public Q_SLOTS:
  /**
   * @brief open start page
   */
  virtual void openStartPage() = 0;

  /**
   * @brief prepare SQL Queries
   */
  virtual void createSearchQuery(const QString &query = QString()) = 0;

  /**
   * @brief create a new entry in this section
   */
  virtual void createNewEntry() = 0;

  /**
   * @brief open Entry with Article ID
   */
  virtual void openEntry(qint64 articleId) = 0;

  /**
   * @brief caller for tab has changed to visible
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
   * @param parent
   */
  explicit TabsIndex(const char *index, QWidget *parent = nullptr);

  /**
   * @brief Fixed Title for Window and Message boxes
   */
  virtual const QString getTitle() const = 0;

  /**
   * @brief Which Stacked index is currently in use?
   * This allows widgets to be set variably
   */
  virtual TabsIndex::ViewPage currentView() = 0;

  /**
   * @brief Create Custom Entries
   */
  virtual bool customAction(const QJsonObject &obj) = 0;

  /**
   * @brief Uniq Tab Index Identifier
   * @return Identifier
   */
  const QString tabIndexId() const;

  bool isClosable();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_INDEX_H
