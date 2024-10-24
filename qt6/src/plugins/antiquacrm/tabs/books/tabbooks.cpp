// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "bookstabconfigwidget.h"
#include "booksindex.h"

TabBooks::TabBooks(QObject *parent) : AntiquaCRM::TabsInterface{parent} {}

const QString TabBooks::displayName() const { return tr("Books"); }

const QString TabBooks::interfaceName() const { return BOOKS_INTERFACE_NAME; }

const QString TabBooks::sqlTableName() const { return BOOKS_SQL_TABLE_NAME; }

const QString TabBooks::sqlFieldPrefix() const {
  return BOOKS_SQL_FIELD_PREFIX;
}

AntiquaCRM::PluginConfigWidget *TabBooks::configWidget(QWidget *parent) const {
  BooksTabConfigWidget *_widget = new BooksTabConfigWidget(parent);
  return _widget;
}

bool TabBooks::addIndexOnInit() const { return BOOKS_ALWAYS_ADD_ONLOAD; }

const QJsonObject TabBooks::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("id", QString(BOOKS_INTERFACE_TABID));
  _jo.insert("title", displayName());
  _jo.insert("tip", tr("Inventory Books"));
  _jo.insert("icon", "kjournal");
  return _jo;
}

AntiquaCRM::TabsIndex *TabBooks::indexWidget(QWidget *parent) const {
  BooksIndex *_w = new BooksIndex(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabBooks::createInterface(QObject *parent) { return (parent != nullptr); }
