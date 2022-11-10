// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORY_H
#define ANTIQUACRM_INVENTORY_H

#include <AGlobal>
#include <QHeaderView>
#include <QIcon>
#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QShortcut>
#include <QStackedWidget>
#include <QTableView>

class Inventory : public QStackedWidget {
  Q_OBJECT
  Q_PROPERTY(bool closable READ isClosable WRITE setClosable NOTIFY sendClosableChanged)

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
   * @brief Set it Closable.
   * @default false
   * @ref closable
   */
  void setClosable(bool b = false);

protected Q_SLOTS:
  void changeEvent(QEvent *event) override;

  /**
   * @brief Copy String to System Clipboard
   */
  void copyToClipboard(const QString &data);

  /**
   * @brief Statusnachricht senden.
   */
  void sendStatusMessage(const QString &message);

Q_SIGNALS:
  /**
   * @brief Send Tab closeable changed
   */
  void sendClosableChanged();

  /**
   * @brief signal from Shortcut @ref m_focusSearch
   */
  void sendSetSearchFocus();

  /**
   * @brief signal from Shortcut @ref m_focusFilter
   */
  void sendSetSearchFilter();

  /**
   * @brief Article Id
   */
  void sendArticleId(qint64 articleId);

  /**
   * @brief Send Article/Customer Ids for Order operations!
   * @param articleId|customerId
   */
  void sendIdToOrder(qint64 currentId);

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
  /**
   * @brief Tabs Inventory
   * @param index Uniq tab Index Name
   */
  explicit Inventory(const char *index, QWidget *parent = nullptr);

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
  static const QIcon getTabIcon(const QString &name = QString("antiqua"));

  Q_INVOKABLE bool isClosable();
  Q_INVOKABLE bool isModified();
};

#endif // ANTIQUACRM_INVENTORY_H
