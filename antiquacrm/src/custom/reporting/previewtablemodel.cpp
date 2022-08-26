// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "previewtablemodel.h"

#include <QDebug>
#include <QSqlField>
#include <QSqlRecord>

PreviewTableModel::PreviewTableModel(QObject *parent) : QSqlQueryModel{parent} {
  setObjectName("preview_table_model");
}

const QString PreviewTableModel::header(const QString &field) const {
  QMap<QString, QString> m;
  m.insert("date", tr("Date"));
  m.insert("invoice", tr("Invoice"));
  m.insert("article", tr("Article"));
  m.insert("price", tr("Price"));
  m.insert("incl", tr("incl."));
  m.insert("vat", tr("VAT"));
  m.insert("ust", tr("UST"));
  m.insert("porto", tr("Porto"));
  m.insert("total", tr("Total"));
  /**
   * @short Helferspalte für CSV export => Excel und OpenLibre
   * @param calc = double
   * @warning !NICHT Ändern!
   */
  m.insert("calc", "calc");
  return m.value(field);
}

QVariant PreviewTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  QSqlRecord r = record(section);
  return header(r.field(section).name());
}
