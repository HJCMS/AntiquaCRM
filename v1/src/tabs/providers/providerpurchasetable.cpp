// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerpurchasetable.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>

ProviderPurchaseTable::ProviderPurchaseTable(QWidget *parent)
    : QTableWidget{parent} {
  setColumnCount(5);
  addHeaderItem(0, tr("Provider"));
  addHeaderItem(1, tr("Article"));
  addHeaderItem(2, tr("Amount"));
  addHeaderItem(3, tr("Price"));
  addHeaderItem(4, tr("Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

const QIcon ProviderPurchaseTable::pic(const QString &name) const {
  return QIcon("://icons/" + name + ".png");
}

void ProviderPurchaseTable::contextMenuEvent(QContextMenuEvent *e) {
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_find =
      m->addAction(pic("action_search"), tr("check article duration"));
  connect(ac_find, SIGNAL(triggered()), SIGNAL(sendCheckArticles()));

  QAction *ac_copy = m->addAction(pic("edit"), tr("copy article id"));
  connect(ac_copy, SIGNAL(triggered()), SLOT(copyIdToClipboard()));

  QAction *ac_open = m->addAction(pic("action_add"), tr("open article id"));
  connect(ac_open, SIGNAL(triggered()), SLOT(prepareOpenArticle()));

  m->exec(e->globalPos());
  delete m;
}

void ProviderPurchaseTable::prepareOpenArticle() {
  qint64 id = getSelectedArticleId();
  if (id > 0)
    emit sendOpenArticle(id);
}

void ProviderPurchaseTable::copyIdToClipboard() {
  QString buf = item(currentItem()->row(), 1)->text();
  QApplication::clipboard()->setText(buf, QClipboard::Clipboard);
}

void ProviderPurchaseTable::addHeaderItem(int i, const QString &name) {
  QTableWidgetItem *item = new QTableWidgetItem(name, QTableWidgetItem::Type);
  setHorizontalHeaderItem(i, item);
}

void ProviderPurchaseTable::setArticleStatus(const QString &article,
                                             bool available) {
  for (int r = 0; r < rowCount(); r++) {
    QTableWidgetItem *article = item(r, 1);
    if (article != nullptr) {
      if (available)
        article->setData(Qt::DecorationRole, pic("action_ok"));
      else
        article->setData(Qt::DecorationRole, pic("action_cancel"));
    }
  }
}

qint64 ProviderPurchaseTable::getSelectedArticleId() {
  QString buf = item(currentItem()->row(), 1)->text();
  bool b = true;
  int id = buf.toInt(&b);
  return (b && id > 0) ? id : 0;
}

QTableWidgetItem *
ProviderPurchaseTable::createItem(const QJsonValue &data) const {
  QString title;
  if (data.type() == QJsonValue::Double) {
    qint64 i = data.toInt();
    title = QString::number(i);
  } else if (data.type() == QJsonValue::Bool) {
    title = (data.toBool()) ? tr("Yes") : tr("No");
  } else {
    title = data.toString().trimmed();
  }

  QTableWidgetItem *item = new QTableWidgetItem(title);
  item->setData(Qt::UserRole, data);
  item->setFlags(Qt::ItemIsEnabled);
  return item;
}

const QStringList ProviderPurchaseTable::getArticleIds() {
  QStringList l;
  for (int r = 0; r < rowCount(); r++) {
    QTableWidgetItem *m = item(r, 1);
    if (m != nullptr) {
      l.append(m->data(Qt::DisplayRole).toString());
    }
  }
  return l;
}
