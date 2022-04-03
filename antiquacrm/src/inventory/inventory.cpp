// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventory.h"
#include "version.h"

#include <QtCore/QDebug>

Inventory::Inventory(int index,QTabWidget *parent) : QWidget{parent} {
  setObjectName("Inventory");
  TabIndex = index;

}

void Inventory::showEvent(QShowEvent *e) {
  switch (e->type()) {
  case QEvent::Show:
    qDebug() << "Inventory::Show" << e->type() << tabTitle() << getTabIndex();
    break;

  case QEvent::Hide:
    qDebug() << "Inventory::Hide" << e->type() << tabTitle() << getTabIndex();
    break;

  case QEvent::FocusIn:
    qDebug() << "Inventory::FocusIn" << e->type() << tabTitle() << getTabIndex();
    break;

  case QEvent::FocusOut:
    qDebug() << "Inventory::FocusOut" << e->type() << tabTitle() << getTabIndex();
    break;

  default:
    qDebug() << __FUNCTION__ << e->type() << tabTitle() << getTabIndex();
  };
}

int Inventory::getTabIndex() { return TabIndex; }

const QString Inventory::tabTitle() { return windowTitle(); }

void Inventory::setTabTitle(const QString &t) {
  if (t.isEmpty())
    return;

  setWindowTitle(t);
}
