// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabssearchbarfilter.h"
#include "antiquaicon.h"

#include <QAbstractItemView>

namespace AntiquaCRM {

TabsSearchBarFilter::TabsSearchBarFilter(QWidget *parent, bool mouseEvents)
    : QComboBox{parent}, wheel_support{mouseEvents} {
  // QComboBox::AdjustToContentsOnFirstShow
  setSizeAdjustPolicy(QComboBox::AdjustToContents);
  setInsertPolicy(QComboBox::NoInsert);
  setEditable(false);
  setMinimumWidth(200);
  setWhatsThis(tr("Select here your optimal search filter."));
  setToolTip(tr("Press CTRL+Shift+F, to quickly open this filter."));

#ifndef Q_OS_LINUX
  QAbstractItemView *_view = view();
  if (_view != nullptr) {
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSelectionMode(QAbstractItemView::SingleSelection);
  }
#endif
}

void TabsSearchBarFilter::wheelEvent(QWheelEvent *e) {
  if (!wheel_support)
    return;

  QComboBox::wheelEvent(e);
}

const QIcon TabsSearchBarFilter::defaultIcon() {
  return AntiquaCRM::antiquaIcon("view-search");
}

const QString TabsSearchBarFilter::currentFilter(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::UserRole).toString();
}

AntiquaCRM::SearchBarFilter TabsSearchBarFilter::getValue(int index) {
  qint8 _i = (index >= 0) ? index : currentIndex();
  return qvariant_cast<AntiquaCRM::SearchBarFilter>(itemData(_i, Qt::UserRole));
}

const QString TabsSearchBarFilter::currentToolTip(int index) {
  int _index = (index < 0) ? currentIndex() : index;
  return itemData(_index, Qt::ToolTipRole).toString();
}

} // namespace AntiquaCRM
