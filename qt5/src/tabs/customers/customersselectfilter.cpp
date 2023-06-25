// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersselectfilter.h"

#include <QIcon>

CustomersSelectFilter::CustomersSelectFilter(QWidget *parent)
    : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  QIcon icon(":icons/view_search.png");

  insertItem(0, icon, tr("Customer"), Filter::Customer);
  setItemData(0, tr("Search Customer by name"), Qt::ToolTipRole);

  insertItem(1, icon, tr("Customer Id"), Filter::CustomerId);
  setItemData(1, tr("Search customer with id"), Qt::ToolTipRole);

  insertItem(2, icon, tr("Company"), Filter::Company);
  setItemData(2, tr("Search only companies"), Qt::ToolTipRole);
}

CustomersSelectFilter::Filter CustomersSelectFilter::filterIndex(int index) {
  int _index = (index != -1) ? index : currentIndex();
  QVariant val = itemData(_index, Qt::UserRole);
  return qvariant_cast<CustomersSelectFilter::Filter>(val);
}

const QJsonObject
CustomersSelectFilter::getFilter(CustomersSelectFilter::Filter filter) {
  QJsonObject obj;
  switch (filter) {
  case (Filter::CustomerId): {
    obj.insert("search", QJsonValue("customer_id"));
    obj.insert("fields", QJsonValue("c_id"));
    break;
  }

  case (Filter::Company): {
    obj.insert("search", QJsonValue("customer_company_name"));
    obj.insert("fields", QJsonValue("c_company_name"));
    break;
  }

  default: // Filter::Customer
    obj.insert("search", QJsonValue("customer_name"));
    obj.insert("fields", QJsonValue("c_firstname,c_lastname,c_company_name"));
    break;
  };

  return obj;
}
