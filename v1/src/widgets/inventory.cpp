// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

InventoryTable::InventoryTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

Inventory::Inventory(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  m_sql = new AntiquaCRM::ASqlCore(this);

  m_focusSearch = new QShortcut(tr("Ctrl+Shift+S", "Search"), this);
  m_focusSearch->setKey(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_S);
  connect(m_focusSearch, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFocus()));

  m_focusFilter = new QShortcut(tr("Ctrl+Shift+F", "Filter"), this);
  m_focusFilter->setKey(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_F);
  connect(m_focusFilter, SIGNAL(activated()), this,
          SIGNAL(sendSetSearchFilter()));

  m_createEntry = new QShortcut(tr("Ctrl+Shift+N", "New Entry"), this);
  m_createEntry->setKey(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);
  connect(m_createEntry, SIGNAL(activated()), this,
          SIGNAL(sendCreateNewEntry()));
}

void Inventory::setClosable(bool b) { closable = b; }

void Inventory::copyToClipboard(const QString &data) {
  QString buf = data.trimmed();
  if (buf.isEmpty())
    return;

  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void Inventory::infoPoUp(const QString &title, const QString &message) {
  QString t = title.isEmpty() ? tr("Information") : title;
  if (message.isEmpty())
    return;

  QMessageBox *m_box = new QMessageBox(QMessageBox::Information, t, message,
                                       QMessageBox::Ok, this, Qt::Popup);
  m_box->exec();
}

void Inventory::warnPoUp(const QString &title, const QString &message) {
  QString t = title.isEmpty() ? tr("Warning") : title;
  if (message.isEmpty())
    return;

  QMessageBox *m_box = new QMessageBox(QMessageBox::Warning, t, message,
                                       QMessageBox::Ok, this, Qt::Popup);
  m_box->exec();
}

bool Inventory::isClosable() { return closable; }

bool Inventory::isModified() { return isWindowModified(); }
