// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchesselectfilter.h"

#include <AntiquaInput>

StitchesSelectFilter::StitchesSelectFilter(QWidget *parent)
    : QComboBox{parent} {
  setWhatsThis(tr("Select search filter."));
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  setToolTip(tr("Press CTRL+Shift+F, to quickly open this Menu."));
  setMinimumWidth(204);

  QString _prefix = tr("Search Prints with") + " ";
  QIcon _icon = AntiquaCRM::antiquaIcon("view-search");

  int _i = 0;
  insertItem(_i, _icon, tr("Title"), // title search
             QString("ip_title,ip_title_extended"));
  setItemData(_i++, _prefix + tr("Search with Title or extended Title!"), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Keywords"), // keyword search
             QString("ip_keyword,ip_storage_compartment"));
  setItemData(_i++, _prefix + tr("Search for Keyword or compartment!"), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Article Id"), // article search
             QString("ip_id"));
  setItemData(_i++, tr("Multiple article ids separated by comma!"), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Author or Artist"), // title
             QString("ip_author"));
  setItemData(_i++, _prefix + tr("Search for Author or Artist."), // tip
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Storage and Keyword"),          // storage
             QString("ip_storage,ip_storage_compartment")); // tip
  setItemData(_i++, tr("In Storage location with Keyword or name."),
              Qt::ToolTipRole);

  insertItem(_i, _icon, tr("Created/Modified in Year"), // date year
             QString("ip_year"));
  setItemData(_i++, tr("Search created or modified in Year!"), // tip
              Qt::ToolTipRole);
}

const QString StitchesSelectFilter::currentFilter(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::UserRole).toString();
}

const QString StitchesSelectFilter::currentToolTip(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::ToolTipRole).toString();
}
