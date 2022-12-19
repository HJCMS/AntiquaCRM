// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthlyreportview.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif

MonthlyReportModel::MonthlyReportModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{"inventory_orders", parent} {
  setObjectName("preview_table_model");
}

const QMap<int, QString> MonthlyReportModel::headerList() const {
  QMap<int, QString> m;
  int i = 0;
  m.insert(i++, tr("Date"));
  m.insert(i++, tr("Invoice"));
  m.insert(i++, tr("Article"));
  m.insert(i++, tr("Price"));
  m.insert(i++, tr("incl."));
  m.insert(i++, tr("VAT"));
  m.insert(i++, tr("UST"));
  m.insert(i++, tr("Porto"));
  m.insert(i++, tr("Total"));
  m.insert(i++, "calc");
  return m;
}

QVariant MonthlyReportModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  return headerList().value(section);
}

MonthlyReportView::MonthlyReportView(QWidget *parent) : QTableView{parent} {
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

  m_model = new MonthlyReportModel(this);
  setModel(m_model);

  m_tableHeader = horizontalHeader();
  m_tableHeader->setDefaultAlignment(Qt::AlignCenter);
  m_tableHeader->setStretchLastSection(true);
}

void MonthlyReportView::setQuery(const QString &query) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query;
#endif
  if (m_model->querySelect(query)) {
    calc_section = m_model->record().indexOf("calc");
    m_tableHeader->hideSection(calc_section);
    resizeColumnsToContents();
    emit sendFinished();
  }
}

const QString MonthlyReportView::headerName(const QString &key) {
  int index = m_model->record().indexOf(key);
  return m_model->headerList().value(index);
}

const QString MonthlyReportView::dataHeader(const QString &delimiter) {
  QStringList list;
  int calc = m_model->record().indexOf("calc");
  for (int i = 0; i < horizontalHeader()->count(); i++) {
    if (i != calc)
      list << m_model->headerData(i, Qt::Horizontal).toString();
  }
  return list.join(delimiter);
}

const QStringList MonthlyReportView::dataRows(const QString &delimiter) {
  int columns = horizontalHeader()->count();
  QStringList list;
  int calcIndex = m_model->record().indexOf("calc");
  for (int r = 0; r < m_model->rowCount(); r++) {
    QStringList cells;
    for (int c = 0; c < columns; c++) {
      if (calcIndex == c)
        continue;

      QModelIndex index = m_model->index(r, c);
      QString buffer = m_model->data(index, Qt::DisplayRole).toString();
      cells << buffer.trimmed();
    }
    list << cells.join(delimiter);
  }
  return list;
}

const QString MonthlyReportView::salesVolume() {
  if (calc_section < 1)
    return QString();

  double sum_price = 0;
  // int columns = horizontalHeader()->count();
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
