// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableview.h"
#include "ainputwidget.h"
#include "orderstabledelegate.h"
#include "orderstablemodel.h"

#include <QAbstractItemView>
#include <QAction>
#include <QFont>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPalette>

OrdersTableView::OrdersTableView(QWidget *parent, bool readOnly)
    : QTableView{parent} {
  setObjectName("OrdersTableView");
  setWindowTitle("Purchases [*]");
  setToolTip(tr("Article purchases"));
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

  m_model = new OrdersTableModel(this);
  setModel(m_model);

  if (!readOnly) {
    // Editoren initialisieren!
    m_delegate = new OrdersTableDelegate(this);
    setItemDelegate(m_delegate);
  }

  QHeaderView *m_header = horizontalHeader();
  m_header->setHighlightSections(true);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_header);

  connect(m_model,
          SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(articleChanged(const QModelIndex &, const QModelIndex &)));
}

void OrdersTableView::changeEvent(QEvent *event) {
  if (event->type() == QEvent::ModifiedChange && isWindowModified())
    parentWidget()->setWindowModified(true);

  qDebug() << Q_FUNC_INFO << isWindowModified();
  QTableView::changeEvent(event);
}

void OrdersTableView::paintEvent(QPaintEvent *event) {
  if (rowCount() == 0) {
    QStringList _info;
    _info << tr("Please insert here the required Order article.");
    _info << tr("An Order requires minimum one Article!");
    QPainter painter(viewport());
    painter.setBrush(palette().text());
    painter.setFont(font());
    painter.setOpacity(0.8);
    painter.drawText(rect(), Qt::AlignCenter, _info.join("\n"));
  }
  QTableView::paintEvent(event);
}

void OrdersTableView::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  if (!p_modelIndex.isValid())
    return;

  QMenu *m = new QMenu("Actions", this);
  QAction *ac_del = m->addAction(AntiquaCRM::antiquaIcon("database-remove"),
                                 tr("Delete selected Article"));
  ac_del->setEnabled((m_model->rowCount() > 1));
  connect(ac_del, SIGNAL(triggered()), SLOT(addDeleteQuery()));

  m->exec(event->globalPos());
  delete m;
}

void OrdersTableView::articleChanged(const QModelIndex &current,
                                     const QModelIndex &previous) {
  Q_UNUSED(current);
  resizeColumnToContents(previous.column());
  horizontalHeader()->setStretchLastSection(true);
  setWindowModified(true);
}

void OrdersTableView::addDeleteQuery() {
  QModelIndex _index = p_modelIndex.sibling(p_modelIndex.row(), 0);
  if (!_index.isValid())
    return;

  qint64 id = m_model->data(_index, Qt::EditRole).toInt();
  if (m_model->removeRow(_index.row())) {
    setWindowModified(true);
    if (id > 0) { // SQL DELETE call
      QString _sql("DELETE FROM article_orders WHERE a_payment_id=");
      _sql.append(QString::number(id) + ";");
      sql_cache << _sql;
    }
  }
}

void OrdersTableView::clearContents() {
  if (rowCount() > 0) {
    m_model->clearContents();
    setWindowModified(true);
  }
  sql_cache.clear();
}

void OrdersTableView::addArticle(const AntiquaCRM::OrderArticleItems &order) {
  if (m_model->addArticle(order)) {
#ifdef ANTIQUA_DEVELOPEMENT
    qInfo("-- OrdersTableView::addArticle --");
#endif
    setWindowModified(true);
  }
}

int OrdersTableView::rowCount() {
  int _c = m_model->rowCount();
#ifdef ANTIQUA_DEVELOPEMENT
  if (_c < 1)
    qWarning("-- OrdersTableView::rowCount=%d --", _c);
#endif
  return _c;
}

bool OrdersTableView::isEmpty() { return (m_model->rowCount() < 1); }

void OrdersTableView::hideColumns(const QStringList &list) {
  foreach (QString fieldName, list) {
    int column = m_model->columnIndex(fieldName);
    horizontalHeader()->setSectionHidden(column, true);
  }
}

bool OrdersTableView::addArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  clearContents();
  if (m_model->addArticles(items)) {
    setWindowModified(false);
    horizontalHeader()->setStretchLastSection(true);
    return true;
  }
  return false;
}

bool OrdersTableView::setOrderId(qint64 orderId) {
  int _count = 0;
  int _index = m_model->columnIndex("a_order_id");
  for (int r = 0; r < m_model->rowCount(); r++) {
    if (m_model->setData(m_model->index(r, _index), orderId, Qt::EditRole))
      _count++;
  }
  return (_count > 0);
}

const QStringList OrdersTableView::getSqlQuery() {
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
        if (value.metaType().id() == QMetaType::QString)
          update << QString(field + "='" + value.toString() + "'");
        else
          update << QString(field + "=" + value.toString());
      } else {
        fields << field;
        if (value.metaType().id() == QMetaType::QString)
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

  if (!sql_cache.isEmpty())
    queries << sql_cache;

  return queries;
}

qint64 OrdersTableView::getPaymentId(int row) {
  if (row >= 0) {
    QModelIndex pIndex = m_model->index(row, 0);
    return m_model->data(pIndex, Qt::EditRole).toInt();
  }
  return -1;
}
