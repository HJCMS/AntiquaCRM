// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "previewtable.h"
#include "previewtablemodel.h"

#include <QDebug>

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

  m_tableHeader = horizontalHeader();
  m_tableHeader->setDefaultAlignment(Qt::AlignCenter);
  m_tableHeader->setStretchLastSection(true);
}

void PreviewTable::setQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query << Qt::endl;
  // m_sql->query(query);
  // qDebug() << m_sql->lastError();
  m_model->setQuery(query, m_sql->db());
  calc_section = m_model->record().indexOf("calc");
  m_tableHeader->hideSection(calc_section);
  resizeColumnsToContents();
}

const QString PreviewTable::headerName(const QString &key) {
  return m_model->header(key);
}

const QString PreviewTable::dataHeader(const QString &delimiter) {
  QStringList list;
  int calcIndex = m_model->record().indexOf("calc");
  for (int i = 0; i < horizontalHeader()->count(); i++) {
    if (i == calcIndex)
      continue;

    list << m_model->headerData(i, Qt::Horizontal).toString();
  }
  return list.join(delimiter);
}

const QStringList PreviewTable::dataRows(const QString &delimiter) {
  double sum_price = 0;
  int columns = horizontalHeader()->count();
  QStringList list;
  int calcIndex = m_model->record().indexOf("calc");
  for (int r = 0; r < m_model->rowCount(); r++) {
    QStringList cells;
    for (int c = 0; c < columns; c++) {
      QModelIndex index = m_model->index(r, c);
      QString buffer = m_model->data(index, Qt::DisplayRole).toString();
      if (calcIndex == c) {
        sum_price += std::stod(buffer.toStdString());
      } else {
        cells << buffer.trimmed();
      }
    }
    list << cells.join(delimiter);
  }
  return list;
}

const QString PreviewTable::salesVolume() {
  if (calc_section < 1)
    return QString();

  double sum_price = 0;
  int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QModelIndex index = m_model->index(r, calc_section);
    QString buffer = m_model->data(index, Qt::DisplayRole).toString();
    sum_price += std::stod(buffer.toStdString());
  }
  char buffer[10];
  int len = std::sprintf(buffer, "%0.2f", sum_price);
  QByteArray array(buffer, len);
  return QString::fromLocal8Bit(array);
}
