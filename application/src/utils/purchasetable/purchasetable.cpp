// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasetable.h"
#include "purchasetabledelegate.h"
#include "purchasetablemodel.h"

#include <QAction>
#include <QFont>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPalette>

PurchaseTable::PurchaseTable(QWidget *parent, bool readOnly)
    : QTableView{parent} {
  setObjectName("purchase_table_view");
  // @note Required for modification calls
  setWindowTitle("Purchases [*]");
  setToolTip(tr("Current article purchases"));
  setSortingEnabled(false);
  setAlternatingRowColors(true);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);

  // Einfaches Auswählen kein Strg+Shift+Mausklick
  setSelectionMode(QAbstractItemView::SingleSelection);

  if (readOnly) {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
  } else {
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::DoubleClicked);
  }

  m_model = new PurchaseTableModel(this);
  setModel(m_model);

  if (!readOnly) {
    // Editoren initialisieren!
    m_delegate = new PurchaseTableDelegate(this);
    setItemDelegate(m_delegate);
  }

  m_headerView = horizontalHeader();
  m_headerView->setHighlightSections(true);
  m_headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_headerView);

  connect(m_model,
          SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(articleChanged(const QModelIndex &, const QModelIndex &)));
}

void PurchaseTable::changeEvent(QEvent *ev) {
  if (ev->type() == QEvent::ModifiedChange && isWindowModified())
    emit sendTableModified(isWindowModified());

  QTableView::changeEvent(ev);
}

void PurchaseTable::paintEvent(QPaintEvent *ev) {
  if (rowCount() == 0) {
    QPainter painter(viewport());
    painter.setBrush(palette().text());
    painter.setFont(font());
    painter.setOpacity(0.8);
    painter.drawText(rect(), Qt::AlignCenter,
                     tr("Please insert here the Ordered article..."));
  }
  QTableView::paintEvent(ev);
}

void PurchaseTable::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  if (!p_modelIndex.isValid())
    return;

  QMenu *m = new QMenu("Actions", this);
  QAction *ac_del = m->addAction(QIcon("://icons/db_remove.png"),
                                 tr("delete selected article"));
  ac_del->setEnabled((m_model->rowCount() > 1));
  connect(ac_del, SIGNAL(triggered()), SLOT(removeArticle()));

  m->exec(event->globalPos());
  delete m;
}

void PurchaseTable::articleChanged(const QModelIndex &current,
                                   const QModelIndex &previous) {
  Q_UNUSED(current);
  resizeColumnToContents(previous.column());
  m_headerView->setStretchLastSection(true);
  setWindowModified(true);
}

void PurchaseTable::removeArticle() {
  QModelIndex child = p_modelIndex.sibling(p_modelIndex.row(), 0);
  if (!child.isValid())
    return;

  qint64 id = m_model->data(child, Qt::EditRole).toInt();
  if (m_model->removeRow(child.row())) {
    setWindowModified(true);
    if (id > 0) { // SQL DELETE call
      QString sql("DELETE FROM article_orders WHERE a_payment_id=");
      sql.append(QString::number(id) + ";");
      sqlToRemoveCache = sql;
    }
  }
}

void PurchaseTable::clearTable() {
  if (rowCount() > 0) {
    m_model->clear();
    setWindowModified(true);
  }
}

void PurchaseTable::addOrderArticle(const AntiquaCRM::OrderArticleItems &item) {
  if (m_model->addArticle(item)) {
#ifdef ANTIQUA_DEVELOPEMENT
    qInfo("-- PurchaseTable WindowModified");
#endif
    setWindowModified(true);
  }
}

int PurchaseTable::rowCount() { return m_model->rowCount(); }

void PurchaseTable::hideColumns(const QStringList &list) {
  foreach (QString fieldName, list) {
    int column = m_model->columnIndex(fieldName);
    m_headerView->setSectionHidden(column, true);
  }
}

bool PurchaseTable::setOrderArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  clearTable();
  sqlToRemoveCache.clear();
  if (m_model->addArticles(items)) {
    setWindowModified(false);
    m_headerView->setStretchLastSection(true);
    return true;
  }
  return false;
}

bool PurchaseTable::setArticleOrderId(qint64 oid) {
  int oid_index = m_model->columnIndex("a_order_id");
  int counter = 0;
  for (int r = 0; r < m_model->rowCount(); r++) {
    if (m_model->setData(m_model->index(r, oid_index), oid, Qt::EditRole))
      counter++;
  }
  return (counter > 0);
}

const QStringList PurchaseTable::getSqlQuery() {
  // Ausgabe
  QStringList queries;
  // Werden bei INSERT|UPDATE Ignoriert!
  QStringList ignoreList({"a_payment_id", "a_modified"});
  // Starte Tabellen abfrage ...
  for (int r = 0; r < m_model->rowCount(); r++) {
    // Suche "a_payment_id" für INSERT|UPDATE Prüfung!
    qint64 pid = m_model->data(m_model->index(r, 0), Qt::EditRole).toInt();
    QStringList update;  /**< SQL UPDATE SET */
    QStringList fields;  /**< SQL INSERT FIELDS */
    QStringList inserts; /**< SQL INSERT VALUES */
    for (int c = 0; c < m_model->columnCount(); c++) {
      QModelIndex index = m_model->index(r, c);
      QString field = m_model->field(index);
      if (ignoreList.contains(field))
        continue;

      QVariant value = m_model->data(index, Qt::EditRole);
      if (pid > 0) {
        if (value.type() == QVariant::String)
          update << QString(field + "='" + value.toString() + "'");
        else
          update << QString(field + "=" + value.toString());
      } else {
        fields << field;
        if (value.type() == QVariant::String)
          inserts << "'" + value.toString() + "'";
        else
          inserts << value.toString();
      }
    }

    if (pid > 0) {
      QString sql("UPDATE article_orders SET ");
      sql.append(update.join(","));
      sql.append(" WHERE a_payment_id=" + QString::number(pid) + ";");
      queries << sql;
      update.clear();
    } else {
      QString sql("INSERT INTO article_orders (");
      sql.append(fields.join(","));
      sql.append(") VALUES (");
      sql.append(inserts.join(","));
      sql.append(");");
      queries << sql;
      fields.clear();
      inserts.clear();
    }
  }

  if (!sqlToRemoveCache.isEmpty())
    queries << sqlToRemoveCache;

  return queries;
}

qint64 PurchaseTable::getArticlePaymentId(int row) {
  if (row >= 0) {
    QModelIndex pIndex = m_model->index(row, 0);
    return m_model->data(pIndex, Qt::EditRole).toInt();
  }
  return -1;
}
