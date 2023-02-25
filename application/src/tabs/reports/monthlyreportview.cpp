// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "monthlyreportview.h"
#include "monthlyreportmodel.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>

static void debugCalculate(QString &id, double calc, double summary) {
  QByteArray arr = id.toLocal8Bit();
  qDebug("Article: %s, Calc: %0.2f, Summary: %0.2f", arr.data(), calc, summary);
}
#endif

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
  calc_section = -1;
  refunds_section = -1;
  // qDebug() << Q_FUNC_INFO << query;
  if (m_model->querySelect(query)) {
    calc_section = m_model->record().indexOf("calc");
    refunds_section = m_model->record().indexOf("refundscost");
#ifndef ANTIQUA_DEVELOPEMENT
    m_tableHeader->hideSection(calc_section);
    m_tableHeader->hideSection(refunds_section);
#endif
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
  for (int i = 0; i < horizontalHeader()->count(); i++) {
    if (calc_section == i || refunds_section == i)
      continue;

    list << m_model->headerData(i, Qt::Horizontal).toString();
  }
  return list.join(delimiter);
}

const QStringList MonthlyReportView::dataRows(const QString &delimiter) {
  int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QStringList cells;
    for (int c = 0; c < columns; c++) {
      if (calc_section == c || refunds_section == c)
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
  if (calc_section < 1 || refunds_section < 1) {
    qWarning("Invalid Report configuration!");
    return QString();
  }

  double sum_price = 0.00;
  // int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QModelIndex calc_index = m_model->index(r, calc_section);
    double calc = m_model->data(calc_index, Qt::EditRole).toDouble();
    sum_price += calc;

    QModelIndex refunds_index = m_model->index(r, refunds_section);
    double refunds = m_model->data(refunds_index, Qt::EditRole).toDouble();
    if (refunds != 0) {
      sum_price += refunds;
    }
#ifdef ANTIQUA_DEVELOPEMENT
    QString article = m_model->data(m_model->index(r, 2)).toString();
    debugCalculate(article, calc, sum_price);
#endif
  }
  char buffer[10];
  int len = std::sprintf(buffer, "%0.2f", sum_price);
  QByteArray array(buffer, len);
  return QString::fromLocal8Bit(array);
}
