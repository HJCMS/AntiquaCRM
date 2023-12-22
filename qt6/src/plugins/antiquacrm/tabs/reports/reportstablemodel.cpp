// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportstablemodel.h"

ReportsTableModel::ReportsTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{"inventory_orders", parent} {
  setObjectName("preview_table_model");
}

const QMap<int, QString> ReportsTableModel::headerIndex() const {
  QMap<int, QString> m;
  int i = 0;
  m.insert(i++, "date");
  m.insert(i++, "invoice");
  m.insert(i++, "article");
  m.insert(i++, "price");
  m.insert(i++, "inclusive");
  m.insert(i++, "vat");
  m.insert(i++, "tax");
  m.insert(i++, "porto");
  m.insert(i++, "total");
  m.insert(i++, "calc");
  m.insert(i++, "refundscost");
  return m;
}

const QMap<int, QString> ReportsTableModel::headerList() const {
  QMap<int, QString> m;
  int i = 0;
  /**
   * @brief Order closed at
   */
  m.insert(i++, tr("Date"));

  /**
   * @brief Invoive No.
   */
  m.insert(i++, tr("Invoice"));

  /**
   * @brief Article Number
   */
  m.insert(i++, tr("Article"));

  /**
   * @brief Article cost
   */
  m.insert(i++, tr("Price"));

  /**
   * @brief With Sales Tax or not?
   */
  m.insert(i++, tr("incl."));

  /**
   * @brief Sales Tax type „sales_tax_type“
   * @short e.g.:19%
   */
  m.insert(i++, tr("VAT"));

  /**
   * @brief Sales Tax value „sales_tax_calc“
   * @short e.g.:0,19€
   */
  m.insert(i++, tr("TAX"));

  /**
   * @brief Delivery package cost
   */
  m.insert(i++, tr("Porto"));

  /**
   * @brief total cost
   */
  m.insert(i++, tr("Total"));

  /**
   * @brief hidden fields
   * @warning NICHT Übersetzen! - Werden bei der Berechnung verwendet!
   */
  m.insert(i++, "calc");
  m.insert(i++, "refundscost");
  return m;
}

QVariant ReportsTableModel::headerData(int section,
                                        Qt::Orientation orientation,
                                        int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return ASqlQueryModel::headerData(section, orientation, role);

  return headerList().value(section);
}

QVariant ReportsTableModel::data(const QModelIndex &item, int role) const {
  if (role == Qt::TextAlignmentRole && item.column() < 9) {
    return QVariant(Qt::AlignRight | Qt::AlignVCenter);
  }
  return ASqlQueryModel::data(item, role);
}
