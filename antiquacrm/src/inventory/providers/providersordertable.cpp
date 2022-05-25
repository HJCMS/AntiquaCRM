// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providersordertable.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHeaderView>

ProvidersOrderTable::ProvidersOrderTable(QWidget *parent)
    : QTableWidget{parent} {
  p_articleColumn = 1;
  setColumnCount(5);
  addHeaderItem(0, tr("Provider"));
  addHeaderItem(p_articleColumn, tr("Article"));
  addHeaderItem(2, tr("Count"));
  addHeaderItem(3, tr("Price"));
  addHeaderItem(4, tr("Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

int ProvidersOrderTable::articleColumn() { return p_articleColumn; }

void ProvidersOrderTable::addHeaderItem(int i, const QString &name) {
  QTableWidgetItem *item = new QTableWidgetItem(name, QTableWidgetItem::Type);
  setHorizontalHeaderItem(i, item);
}

const QVariant ProvidersOrderTable::getData(int row, int column) {
  QVariant data;
  QTableWidgetItem *it = item(row, column);
  if (it != nullptr) {
    return it->data(Qt::DisplayRole);
  }
  return data;
}

QTableWidgetItem *ProvidersOrderTable::createItem(const QString &title) const {
  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setFlags(Qt::ItemIsEnabled); // Kein Editieren zulassen!
  return item;
}
