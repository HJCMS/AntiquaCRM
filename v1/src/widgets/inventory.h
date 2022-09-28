// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORY_H
#define ANTIQUACRM_INVENTORY_H

#include <QObject>
#include <QIcon>
#include <QShortcut>
#include <QTableView>
#include <QHeaderView>
#include <QStackedWidget>

class Inventory : public QStackedWidget {
  Q_OBJECT
  Q_PROPERTY(bool closable READ isClosable WRITE setClosable NOTIFY
                 sendCloseableChanged)

private:
  QShortcut *m_focusSearch; /**< @brief Ctrl+Shift+S */
  QShortcut *m_focusFilter; /**< @brief Ctrl+Shift+F */
  QShortcut *m_createEntry; /**< @brief Ctrl+Shift+N */
  void addShortCutsAndSignals();

protected:
  bool initialed = false;
  bool closable = true;
  void setClosable(bool b = false);

protected Q_SLOTS:
  void changeEvent(QEvent *event) override;
  void copyToClipboard(const QString &data);
  void infoPoUp(const QString &title, const QString &message);
  void warnPoUp(const QString &title, const QString &message);

Q_SIGNALS:
  void sendCloseableChanged(bool);
  void sendSetSearchFocus();
  void sendSetSearchFilter();
  void sendCreateNewEntry();
  void sendEnabledStatus(bool);

public Q_SLOTS:
  virtual void createEntry() = 0;
  virtual void openEntry(qint64 id) = 0;
  virtual void onEnterChanged() = 0;

public:
  explicit Inventory(QWidget *parent = nullptr);
  static const QIcon defaultIcon();
  Q_INVOKABLE bool isClosable();
  Q_INVOKABLE bool isModified();
};

#endif // ANTIQUACRM_INVENTORY_H
