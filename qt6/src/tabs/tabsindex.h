// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_TABS_INDEX_H
#define ANTIQUA_TABS_INDEX_H

#include <AGlobal>
#include <QEvent>
#include <QObject>
#include <QShortcut>
#include <QStackedWidget>

class TabsIndex : public QStackedWidget {
  Q_OBJECT
  Q_PROPERTY(bool closable READ isClosable WRITE setClosable NOTIFY
                 sendClosableChanged)

private:
  const QString tabIndex;   /**< @brief Uniq Tab Index Name */
  QShortcut *m_focusSearch; /**< @brief Ctrl+Shift+S */
  QShortcut *m_focusFilter; /**< @brief Ctrl+Shift+F */
  QShortcut *m_createEntry; /**< @brief Ctrl+Shift+N */
  void addShortCutsAndSignals();

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
   * @brief Wenn Editor nicht gespeicherte Datensätze meldet!
   * @note Muss mit obj->installEventFilter(this) Installiert sein!
   */
  bool eventFilter(QObject *, QEvent *) override;

  /**
   * @brief Set it Closable.
   * @ref closable
   */
  void setClosable(bool b = false);

protected Q_SLOTS:
  /**
   * @brief Activ Widget or not ?
   */
  void changeEvent(QEvent *) override;

  /**
   * @brief Copy String to System Clipboard
   */
  void copyToClipboard(const QString &data);

  /**
   * @brief Statusnachricht senden.
   */
  void sendStatusMessage(const QString &message);

  /**
   * @brief Befehl an den Socket
   */
  void sendSocketOperation(const QJsonObject &obj);

  /**
   * @brief Hinweisfenster öffnen!
   */
  void openWarningPopUp(const QString &title, const QString &message);

  virtual void popupWarningTabInEditMode() = 0;

  /**
   * @brief Standard Tabellenabfrage aufrufen
   */
  virtual void setDefaultTableView() = 0;

Q_SIGNALS:
  /**
   * @brief Speichernstände mitteilen
   */
  void sendUnsafedChanges(bool);

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

  /**
   * @brief Emitted when Tab change to active!
   * @code
   *  (event->type() == QEvent::EnabledChange)
   * @endcode
   */
  void sendEnabledStatus(bool);

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
  enum ViewIndex { MainView = 0, EditorView = 1, CustomView = 2 };

  /**
   * @brief Tabs Inventory
   * @param index - Uniq tab Index Name
   * @param parent
   */
  explicit TabsIndex(const char *index, QWidget *parent = nullptr);

  /**
   * @brief Which Stacked index is currently in use?
   * This allows widgets to be set variably
   */
  virtual TabsIndex::ViewIndex currentView() = 0;

  /**
   * @brief Create Custom Entries
   */
  virtual bool customAction(const QJsonObject &obj) = 0;

  /**
   * @brief Uniq Tab Index Identifier
   * @return Identifier
   */
  const QString tabIndexId() const;

  /**
   * @brief get Icon fpr this Tab
   */
  static const QIcon getTabIcon(const QString &name = QString("antiquacrm"));

  bool isClosable();
};

#endif // ANTIQUA_TABS_INDEX_H
