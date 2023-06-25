// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersselectfilter.h"

#include <AntiquaInput>

CustomersSelectFilter::CustomersSelectFilter(QWidget *parent)
    : QComboBox{parent} {
  setWhatsThis(tr("Select search filter."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  setToolTip(tr("Press CTRL+Shift+F, to quickly open this Menu."));
  setMinimumWidth(204);

  QIcon _icon = AntiquaCRM::AntiquaApplIcon("view-search");

  int _i = 0;
  insertItem(_i, _icon, tr("Customer"), // customer search
             QString("c_firstname,c_lastname,c_company_name"));
  setItemData(_i++, tr("Search customer or company"), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Street or Location"), // location search
             QString("c_street,c_location"));
  setItemData(_i++, tr("Search by Street or Location."), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Customer Id"), // article search
             QString("c_id"));
  setItemData(_i++, tr("Multiple article ids separated by comma!"), // tip
              Qt::ToolTipRole);
}

const QString CustomersSelectFilter::getFilter(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::UserRole).toString();
}

const QString CustomersSelectFilter::getToolTip(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::ToolTipRole).toString();
}
