// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"
#include "messagebox.h"

Inventory::Inventory(QWidget *parent) : QWidget{parent} {
  if (objectName().isEmpty())
    setObjectName("Inventory");
}

void Inventory::displayMessageBox(const QString &msg) {
  MessageBox *box = new MessageBox(this);
  box->notice(msg);
}

void Inventory::setClosable(bool b) { closable = ((b) ? false : true); }

bool Inventory::isClosable() { return closable; }
