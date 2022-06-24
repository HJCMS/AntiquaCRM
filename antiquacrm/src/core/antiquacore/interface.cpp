// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "interface.h"

#include <QApplication>
#include <QAction>
#include <QClipboard>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>
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

void PurchaserOrderTable::contextMenuEvent(QContextMenuEvent *e) {
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_remove = m->addAction(
      style()->standardIcon(QStyle::SP_FileIcon), tr("inspect article"));
  ac_remove->setObjectName("ac_context_search_article");
  connect(ac_remove, SIGNAL(triggered()), this, SIGNAL(findArticleNumbers()));
  QAction *ac_copy = m->addAction(
      style()->standardIcon(QStyle::SP_FileIcon), tr("copy article id"));
  ac_copy->setObjectName("ac_context_ac_copy_article");
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyIdToClipboard()));

  m->exec(e->globalPos());
  delete m;
}

void PurchaserOrderTable::copyIdToClipboard()
{
  QString buf = item(currentItem()->row(),1)->text();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
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
