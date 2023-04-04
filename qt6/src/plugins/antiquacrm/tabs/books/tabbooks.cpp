// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "booksconfig.h"
#include "bookswidget.h"

TabBooks::TabBooks(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("plugin_tab_books");
}

const QString TabBooks::displayName() const { return tr("Books"); }

const QString TabBooks::interfaceName() const { return BOOKS_INTERFACE_NAME; }

const QString TabBooks::sqlTableName() const { return BOOKS_SQL_TABLE_NAME; }

const QString TabBooks::sqlFieldPrefix() const {
  return BOOKS_SQL_FIELD_PREFIX;
}

QWidget *TabBooks::configWidget(QWidget *parent) const {
  Q_UNUSED(parent);
  return nullptr;
}

AntiquaCRM::TabsIndex *TabBooks::indexWidget(QWidget *parent) const {
  BooksWidget *_widget = new BooksWidget(parent);
  return _widget;
}

bool TabBooks::createInterface(QObject *parent) {
  // TODO
  return (parent != nullptr);
}
