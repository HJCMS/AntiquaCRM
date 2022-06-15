// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"
#include "messagebox.h"
#include "utilsmain.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QShortcut>

Inventory::Inventory(QWidget *parent) : QWidget{parent} {
  if (objectName().isEmpty())
    setObjectName("Inventory");

  QShortcut *focusSearch = new QShortcut(tr("Ctrl+S", "Search"), this);
  focusSearch->setKey(Qt::ControlModifier + Qt::Key_S);
  connect(focusSearch, SIGNAL(activated()), this, SIGNAL(s_setSearchFocus()));

  QShortcut *focusFilter = new QShortcut(tr("Ctrl+F", "Filter"), this);
  focusFilter->setKey(Qt::ControlModifier + Qt::Key_F);
  connect(focusFilter, SIGNAL(activated()), this, SIGNAL(s_setSearchFilter()));

  QShortcut *createEntry = new QShortcut(tr("Ctrl+N", "New Entry"), this);
  createEntry->setKey(Qt::ControlModifier + Qt::Key_N);
  connect(createEntry, SIGNAL(activated()), this, SIGNAL(s_createNewEntry()));
}

void Inventory::displayMessageBox(const QString &msg) {
  MessageBox *box = new MessageBox(this);
  box->notice(msg);
}

void Inventory::copyIntoClipboard(const QVariant &val) {
  QString buf = val.toString();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void Inventory::setClosable(bool b) { closable = b; }

void Inventory::setIsModified(bool b) {
  setWindowModified(b);
  emit s_windowModified(b);
}

bool Inventory::isClosable() { return closable; }

void Inventory::findModifySignals() {
  QList<UtilsMain *> l =
      findChildren<UtilsMain *>(QString(), Qt::FindChildrenRecursively);
  if (l.count() > 1) {
    for (int i = 0; i < l.count(); i++) {
      UtilsMain *w = l.at(i);
      if (w != nullptr) {
        connect(w, SIGNAL(hasModified(bool)), this, SLOT(setIsModified(bool)));
      }
    }
  }
}
