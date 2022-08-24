// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "previewtable.h"
#include "previewtablemodel.h"

#include <QDebug>
#include <QHeaderView>

#ifndef TABLE_PRICE_CELL
#define TABLE_PRICE_CELL 4
#endif

PreviewTable::PreviewTable(QWidget *parent) : QTableView{parent} {
  setObjectName("reporting_table");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_sql = new HJCMS::SqlCore(this);

  m_model = new PreviewTableModel(this);
  m_model->setObjectName("preview_table_model");
  setModel(m_model);

  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);
}

void PreviewTable::setQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query << Qt::endl;
  m_model->setQuery(query, m_sql->db());
  resizeColumnsToContents();
}

const QString PreviewTable::dataHeader(const QString &delimiter) {
  QStringList list;
  QHeaderView *header = horizontalHeader();
  for (int i = 0; i < header->count(); i++) {
    QString t = m_model->headerData(i, Qt::Horizontal).toString();
    list.append(t.trimmed().toUpper());
  }
  return list.join(delimiter);
}

const QStringList PreviewTable::dataRows(const QString &delimiter) {
  double sum_price = 0;
  int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QStringList cells;
    for (int c = 0; c < columns; c++) {
      QModelIndex index = m_model->index(r, c);
      QString buffer = m_model->data(index, Qt::DisplayRole).toString();
      if (c == TABLE_PRICE_CELL) {
        sum_price += std::stod(buffer.toStdString());
      }
      cells << buffer.trimmed();
    }
    list << cells.join(delimiter);
  }
  QStringList summary;
  for (int c = 0; c < columns; c++) {
    if (c == TABLE_PRICE_CELL) {
      char buffer[10];
      int len = std::sprintf(buffer, "%0.2f", sum_price);
      QByteArray array(buffer, len);
      summary << QString::fromLocal8Bit(array);
    } else {
      summary << QString();
    }
  }
  list << summary.join(delimiter);
  return list;
}

const QString PreviewTable::salesVolume() {
  double sum_price = 0;
  int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QModelIndex index = m_model->index(r, TABLE_PRICE_CELL);
    QString buffer = m_model->data(index, Qt::DisplayRole).toString();
    sum_price += std::stod(buffer.toStdString());
  }
  char buffer[10];
  int len = std::sprintf(buffer, "%0.2f", sum_price);
  QByteArray array(buffer, len);
  return QString::fromLocal8Bit(array);
}
