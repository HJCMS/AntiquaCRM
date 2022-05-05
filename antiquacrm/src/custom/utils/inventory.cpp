// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"
#include "messagebox.h"

#include <QDebug>
#include <QApplication>
#include <QClipboard>

Inventory::Inventory(QWidget *parent) : QWidget{parent} {
  if (objectName().isEmpty())
    setObjectName("Inventory");
}

void Inventory::displayMessageBox(const QString &msg) {
  MessageBox *box = new MessageBox(this);
  box->notice(msg);
}

void Inventory::copyIntoClipboard(const QVariant &val)
{
  QString buf = val.toString();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void Inventory::setClosable(bool b) { closable = ((b) ? false : true); }

bool Inventory::isClosable() { return closable; }
