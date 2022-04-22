// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"

Inventory::Inventory(QWidget *parent) : QWidget{parent} {
  if (objectName().isEmpty())
    setObjectName("Inventory");
}

void Inventory::setClosable(bool b) { closable = ((b) ? false : true); }

bool Inventory::isClosable() { return closable; }
