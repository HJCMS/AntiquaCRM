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
    : QTableView{parent} {
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSortingEnabled(false);
  setSelectionMode(QAbstractItemView::SingleSelection);
  if (readOnly) {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
  } else {
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setEditTriggers(QAbstractItemView::DoubleClicked);
  }

  AntiquaCRM::ASettings cfg(this);

  m_model = new PurchaseTableModel(this);
  // m_model->setEditStrategy(QSqlTableModel::OnRowChange);
  setModel(m_model);

  if (!readOnly) {
    EditorProperties config;
    config.minPrice = cfg.value("payment/min_price", 5.00).toDouble();
    config.maxPrice = cfg.value("payment/max_price", 999999.00).toDouble();
    config.minCount = cfg.value("payment/min_count", 1).toInt();
    config.maxCount = cfg.value("payment/max_count", 10).toInt();
    config.currency = cfg.value("payment/currency", "€").toByteArray();
    config.maxInteger = cfg.value("payment/max_integer", 9999999).toInt();

    m_delegate = new PurchaseTableDelegate(config, this);
    setItemDelegate(m_delegate);
  }

  m_headerView = horizontalHeader();
  m_headerView->setHighlightSections(true);
  m_headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_headerView);
}

bool PurchaseTable::removeRow(int row) {
  // TODO
  return true;
}

void PurchaseTable::clearTable() {
  if (m_model->rowCount() > 0)
    m_model->clear();
}

void PurchaseTable::addOrderArticle(const AntiquaCRM::OrderArticleItems &item) {
  m_model->addModelData(item);
}

void PurchaseTable::hideColumns(const QList<int> &list) {
  QListIterator<int> i(list);
  while (i.hasNext()) {
    m_headerView->setSectionHidden(i.next(), true);
  }
  m_headerView->setStretchLastSection(true);
}

bool PurchaseTable::setOrderArticles(
    const QList<AntiquaCRM::OrderArticleItems> &items) {
  clearTable();
  return m_model->setModelData(items);
}

const QStringList PurchaseTable::getQueryArticles() {
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

  return queries;
}
