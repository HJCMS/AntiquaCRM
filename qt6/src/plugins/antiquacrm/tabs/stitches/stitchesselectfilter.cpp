// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchesselectfilter.h"

StitchesSelectFilter::StitchesSelectFilter(QWidget *parent)
    : AntiquaCRM::TabsSearchBarFilter{parent} {
  setObjectName("stitches_filter_selecter");
  createItemData();
}

void StitchesSelectFilter::createItemData() {
  int _i = 0;
  const QString _prefix(tr("Searches prints, stitches and engravings") + " ");

  insertItem(_i, defaultIcon(), tr("Title"),
             QString("ip_title,ip_title_extended"));
  setItemData(_i++, _prefix + tr("in title or extended title!"),
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Keywords"),
             QString("ip_keyword,ip_storage_compartment"));
  setItemData(_i++, _prefix + tr("in keywords or storage compartment!"),
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Article No."), QString("ip_id"));
  setItemData(_i++, tr("Multiple article numbers separated by comma!"),
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Authors or Artists"), QString("ip_author"));
  setItemData(_i++, _prefix + tr("in authors or artists field."), // tip
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Keyword- or Storage location"),
             QString("ip_storage,ip_storage_compartment")); // tip
  setItemData(_i++, tr("Searches in storage location or in storage keywords."),
              Qt::ToolTipRole);

  insertItem(_i, defaultIcon(), tr("Created/modified in year"),
             QString("ip_year"));
  setItemData(_i++, tr("Searches by created or modified in Year!"),
              Qt::ToolTipRole);
}
