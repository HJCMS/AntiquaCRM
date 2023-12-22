// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportstableview.h"
#include "reportstablemodel.h"

#include <QChar>
#include <QJsonArray>
#include <QJsonValue>
#include <QPainter>
#include <QTime>

#ifdef DEBUG_REPORTS_VIEW
#include <QDebug>

static void debugCalculate(QString &id, double calc, double summary) {
  QByteArray arr = id.toLocal8Bit();
  qDebug("Article: %s, Calc: %0.2f, Summary: %0.2f", arr.data(), calc, summary);
}
#endif

ReportsTableView::ReportsTableView(QWidget *parent) : QTableView{parent} {
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

  m_model = new ReportsTableModel(this);
  setModel(m_model);

  m_tableHeader = horizontalHeader();
  m_tableHeader->setDefaultAlignment(Qt::AlignCenter);
  m_tableHeader->setStretchLastSection(false);
}

void ReportsTableView::paintEvent(QPaintEvent *ev) {
  if (m_model->rowCount() == 0) {
    QPainter painter(viewport());
    painter.setBrush(palette().text());
    painter.setFont(font());
    painter.setOpacity(0.8);
    painter.drawText(rect(), Qt::AlignCenter,
                     tr("No result for current selection."));
  }
  QTableView::paintEvent(ev);
}

void ReportsTableView::setQuery(const QString &query) {
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

const QString ReportsTableView::headerName(const QString &key) {
  int index = m_model->record().indexOf(key);
  return m_model->headerList().value(index);
}

const QString ReportsTableView::dataHeader(const QChar &delimiter) {
  QStringList list;
  for (int i = 0; i < horizontalHeader()->count(); i++) {
    if (calc_section == i || refunds_section == i)
      continue;

    list << m_model->headerData(i, Qt::Horizontal).toString();
  }
  return list.join(delimiter);
}

const QStringList ReportsTableView::dataRows(const QChar &delimiter) {
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

const QStringList ReportsTableView::csvExport(const QChar &delimiter) {
  int columns = horizontalHeader()->count();
  QStringList list;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QStringList cells;
    for (int c = 0; c < columns; c++) {
      if (calc_section == c || refunds_section == c)
        continue;

      QModelIndex _index = m_model->index(r, c);
      QString _buffer = m_model->data(_index, Qt::DisplayRole).toString();
      _buffer = _buffer.trimmed();
      if (c > 0 && c < 3) { // Invoice && Article ID's
        _buffer.prepend(QChar(34));
        _buffer.append(QChar(34));
        cells << _buffer;
      } else {
        cells << _buffer;
      }
    }
    list << cells.join(delimiter);
  }
  return list;
}

double ReportsTableView::salesVolume() {
  if (calc_section < 1 || refunds_section < 1) {
    qWarning("Invalid Report configuration!");
    return 0.00;
  }

  double sum_price = 0.00;
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
  }
  return sum_price;
}

const QJsonObject ReportsTableView::printingData() {
  QJsonObject _obj;
  QMap<int, QString> _map = m_model->headerIndex();
  // Header
  QJsonObject _header;
  for (int c = 0; c < m_model->columnCount(); c++) {
    if (calc_section == c || refunds_section == c)
      continue;

    _header.insert(_map[c], m_model->headerData(c, Qt::Horizontal).toString());
  }
  _obj.insert("header", _header);
  // Sold
  QJsonArray _arr;
  for (int r = 0; r < m_model->rowCount(); r++) {
    QJsonObject _jso;
    for (int c = 0; c < m_model->columnCount(); c++) {
      if (calc_section == c || refunds_section == c)
        continue;

      const QModelIndex _index = m_model->index(r, c);
      QVariant _value = m_model->data(_index, Qt::EditRole);
      if (_map[c].startsWith("date")) {
        const QString _str = _value.toDate().toString("dd.MM.yyyy");
        _jso.insert(_map[c], QJsonValue(_str));
      } else {
        _jso.insert(_map[c], _value.toJsonValue());
      }
    }
    _arr.append(_jso);
  }
  _obj.insert("sold", _arr);
  _obj.insert("total", salesVolume());
  _obj.insert("money", moneyVolume());
  return _obj;
}

const QString ReportsTableView::moneyVolume() {
  return AntiquaCRM::ATaxCalculator::money(salesVolume());
}
