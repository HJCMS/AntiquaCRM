// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "previewtablemodel.h"
#include "applsettings.h"

static const QString getCurrency() {
  ApplSettings cfg;
  return cfg.value("payment/currency", "€").toString();
}

PreviewTableModel::PreviewTableModel(QObject *parent)
    : QSqlQueryModel{parent}, p_currency(getCurrency()) {
  setObjectName("preview_table_model");
}

QVariant PreviewTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);

  switch (index.column()) {
  case 0: // invoice_id
  {
    QString str = item.toString();
    return str.rightJustified(7, '0');
  }

  case 3: // vat
  {
    QString str = item.toString();
    if (item.toInt() == 0)
      return QString();

    return str + "%";
  }

  case 4: // price
  {
    QString str = QString::number(item.toInt(), 'f', 2);
    str.append(" ");
    str.append(p_currency);
    return str;
  }

  default:
    return item;
  }
}

QVariant PreviewTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (section) {
  case 0: // Invoice
    return tr("Invoice");

  case 1: // article
    return tr("Article");

  case 2: // title
    return tr("Title");

  case 3: // vat
    return tr("VAT");

  case 4: // price
    return tr("Price");

  case 5: // delivered
    return tr("Delivered");

  default:
    return QString("%1").arg(section);
  }
  return QString("%1").arg(section);
}
