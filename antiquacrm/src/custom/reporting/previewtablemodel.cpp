// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "previewtablemodel.h"
#include "applsettings.h"

#include <QDebug>

static const QString getCurrency() {
  ApplSettings cfg;
  return cfg.value("payment/currency", "€").toString();
}

PreviewTableModel::PreviewTableModel(QObject *parent)
    : QSqlQueryModel{parent}, p_currency(getCurrency()) {
  setObjectName("preview_table_model");
}

const QString PreviewTableModel::currencyString(const QVariant &val) const {
  bool b;
  double d = val.toDouble(&b);
  if (b && d > 0) {
    char buffer[10];
    int len = std::sprintf(buffer, "%0.2f", d);
    QByteArray array(buffer, len);
    return QString::fromLocal8Bit(array);
  }
  return QString();
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

  case 2: // title
  {
    QString str = item.toString();
    if (str.length() > 30) {
      QStringRef ref = str.leftRef(30);
      str = ref.trimmed().toString();
      str.append("...");
    }
    return str;
  }

  case 3: // vat
    return item.toInt();

  case 4: // price
    return currencyString(item);

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
  {
    QString str(tr("VAT"));
    str.append(" %");
    return str;
  }

  case 4: // price
  {
    QString str(tr("Price"));
    str.append(" ");
    str.append(p_currency);
    return str;
  }

  case 5: // delivered
    return tr("Delivered");

  default:
    return QString("%1").arg(section);
  }
  return QString("%1").arg(section);
}
