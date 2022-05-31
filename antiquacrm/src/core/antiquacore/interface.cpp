// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interface.h"

#include <QHeaderView>
#include <QIcon>
#include <QStyle>

namespace Antiqua {

PurchaserOrderTable::PurchaserOrderTable(QWidget *parent)
    : QTableWidget{parent} {
  setColumnCount(5);
  addHeaderItem(0, tr("Provider"));
  addHeaderItem(1, tr("Article"));
  addHeaderItem(2, tr("Count"));
  addHeaderItem(3, tr("Price"));
  addHeaderItem(4, tr("Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

void PurchaserOrderTable::addHeaderItem(int i, const QString &name) {
  QTableWidgetItem *item = new QTableWidgetItem(name, QTableWidgetItem::Type);
  item->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  setHorizontalHeaderItem(i, item);
}

QTableWidgetItem *PurchaserOrderTable::createItem(const QString &title) const {
  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setFlags(Qt::ItemIsEnabled);
  return item;
}

PurchaserWidget::PurchaserWidget(QWidget *parent) : QGroupBox{parent} {
  setToolTip(tr("purchaser"));
  setFlat(true);
  setStyleSheet("QGroupBox {border:none;}");
}

int PurchaserWidget::customerId() { return id; };

ProviderWidget::ProviderWidget(const QString &widgetId, QWidget *parent)
    : QWidget{parent} {}

InterfaceWidget::InterfaceWidget(const QString &widgetId, QWidget *parent)
    : QScrollArea{parent} {}

const QString InterfaceWidget::sqlParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.key() == key)
      return fi.value();
  }
  return QString();
}

const QString InterfaceWidget::apiParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.value() == key)
      return fi.key();
  }
  return QString();
}

}; // namespace Antiqua
