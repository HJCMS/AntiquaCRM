// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "booksconfig.h"
#include "bookstabconfigwidget.h"
#include "bookswidget.h"

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
  _jo.insert("title", tr("Books"));
  _jo.insert("id", QString(BOOKS_INTERFACE_TABID));
  _jo.insert("tip", tr("Inventory Books"));
  _jo.insert("icon", tr("kjournal"));
  return _jo;
}

AntiquaCRM::TabsIndex *TabBooks::indexWidget(QWidget *parent) const {
  BooksWidget *_widget = new BooksWidget(parent);
  Q_CHECK_PTR(_widget);
  return _widget;
}

bool TabBooks::createInterface(QObject *parent) {
  // TODO
  return (parent != nullptr);
}
