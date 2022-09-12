// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchaserordertable.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>

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
  QAction *ac_remove = m->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                                    tr("inspect article"));
  ac_remove->setObjectName("ac_context_search_article");
  connect(ac_remove, SIGNAL(triggered()), this, SIGNAL(findArticleNumbers()));

  QAction *ac_copy = m->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                                  tr("copy article id"));
  ac_copy->setObjectName("ac_context_ac_copy_article");
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyIdToClipboard()));

  QAction *ac_open = m->addAction(style()->standardIcon(QStyle::SP_FileIcon),
                                  tr("open article id"));
  ac_open->setObjectName("ac_context_ac_open_article");
  connect(ac_open, SIGNAL(triggered()), this, SLOT(findSelectedArticleId()));

  m->exec(e->globalPos());
  delete m;
}

void PurchaserOrderTable::findSelectedArticleId() {
  QString buf = item(currentItem()->row(), 1)->text();
  bool b = true;
  int id = buf.toInt(&b);
  if (b && id > 0)
    emit inspectArticle(id);
}

void PurchaserOrderTable::copyIdToClipboard() {
  QString buf = item(currentItem()->row(), 1)->text();
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

}; // namespace Antiqua
