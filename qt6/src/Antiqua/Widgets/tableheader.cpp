// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tableheader.h"

namespace AntiquaCRM {

TableHeader::TableHeader(QWidget *parent)
    : QHeaderView{Qt::Horizontal, parent} {
  setSectionsMovable(false);
  setDefaultAlignment(Qt::AlignCenter);
  setSectionResizeMode(QHeaderView::Interactive);
}

void TableHeader::resizeToContents(bool b) {
  if (b)
    resizeSections(QHeaderView::ResizeToContents);
}

} // namespace AntiquaCRM
