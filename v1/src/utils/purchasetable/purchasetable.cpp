// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetable.h"
#include "purchasetabledelegate.h"
#include "purchasetablemodel.h"

#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QTableWidgetItem>

PurchaseTable::PurchaseTable(QWidget *parent, bool readOnly)
    : QTableView{parent}, readOnlyMode{readOnly} {
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSortingEnabled(false);
  setSelectionMode(QAbstractItemView::SingleSelection);
  if (readOnlyMode) {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
  } else {
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::DoubleClicked);
  }

  m_sql = new AntiquaCRM::ASqlCore(this);

  m_model = new PurchaseTableModel(m_sql, this);
  m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
  setModel(m_model);

  AntiquaCRM::ASettings cfg(this);

  EditorProperties config;
  config.minPrice = cfg.value("payment/min_price", 5.00).toDouble();
  config.maxPrice = cfg.value("payment/max_price", 999999.00).toDouble();
  config.minCount = cfg.value("payment/min_count", 1).toInt();
  config.maxCount = cfg.value("payment/max_count", 10).toInt();
  config.currency = cfg.value("payment/currency", "€").toByteArray();

  m_delegate = new PurchaseTableDelegate(config, this);
  if (!readOnlyMode) {
    qInfo("init PurchaseTableDelegate");
    setItemDelegate(m_delegate);
  }

  m_header = horizontalHeader();
  m_header->setHighlightSections(true);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_header);
}

void PurchaseTable::contextMenuEvent(QContextMenuEvent *event) {
  const QModelIndex index = indexAt(event->pos());
  if (index.isValid()) {
    QMenu *m = new QMenu("Actions", this);
    QAction *ac_del =
        m->addAction(QIcon("://icons/db_remove.png"), tr("Remove"));
    connect(ac_del, SIGNAL(triggered()), SLOT(removeCurrentRow()));
    m->exec(event->globalPos());
    delete m;
  }
}

void PurchaseTable::removeCurrentRow() {
  int r = 0;
  QModelIndex index;
  foreach (QModelIndex i, selectedIndexes()) {
    r = i.row();
    index = i;
    break;
  }

  QString info("<p>");
  info.append(tr("Do you really want to remove this Entry from the list?"));
  info.append("</p><p>");
  info.append(tr("This operation is not reversible!"));
  info.append("</p>");
  int ret = QMessageBox::question(this, tr("Remove Article"), info);
  if (ret == QMessageBox::Yes) {
    if (m_model->removeRow(r, index))
      m_model->reload();
  }
}

void PurchaseTable::setQueryId(qint64 id, const QString &field) {
  m_model->setQueryId(field, id);
}

void PurchaseTable::hideColumns(const QList<int> columns) {
  QListIterator<int> it(columns);
  while (it.hasNext()) {
    int i = it.next();
    if (i >= 0 && !m_model->editableColumns().contains(i))
      m_header->setSectionHidden(i, true);
  }
  m_header->setStretchLastSection(true);
}

bool PurchaseTable::addRow(qint64 orderId,
                           const AntiquaCRM::OrderArticleItems &items) {
  QSqlRecord record = m_sql->record(m_model->tableName());
  m_model->insertRows(verticalHeader()->count(), 1, QModelIndex());
  QModelIndex index = indexAt(QPoint(1, 1));
  if (!index.isValid()) {
    qWarning("PurchaseTable:Invalid ModelIndex");
    return false;
  }

  bool ret = false;
  int row = (verticalHeader()->count() - 1);
  QListIterator<AntiquaCRM::ArticleOrderItem> it(items);
  int count = 0;
  while (it.hasNext()) {
    AntiquaCRM::ArticleOrderItem article = it.next();
    int column = record.indexOf(article.key);
    QModelIndex sub = index.sibling(row, column);
    if (sub.isValid() && column >= 0) {
      ret = m_model->setData(sub, article.value, Qt::EditRole);
      if(ret)
        count++;
    }
  }
  return (count == m_model->record().count());
}

const AntiquaCRM::OrderArticleItems PurchaseTable::getRow(int row) {
  AntiquaCRM::OrderArticleItems items;
  //
  return items;
}
