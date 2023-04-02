// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acombobox.h"
#include "abstractinput.h"

#include <QAbstractItemView>
#include <QIcon>

namespace AntiquaCRM {

AComboBox::AComboBox(QWidget *parent, bool mouseEvents)
    : QComboBox{parent}, wheel_support{mouseEvents} {
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setInsertPolicy(QComboBox::NoInsert);
  setEditable(false);
#ifndef Q_OS_LINUX
  QAbstractItemView *_view = view();
  if (_view != nullptr) {
    _view->setAlternatingRowColors(true);
    _view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _view->setSelectionMode(QAbstractItemView::SingleSelection);
  }
#endif
}

void AComboBox::wheelEvent(QWheelEvent *e) {
  if (!wheel_support)
    return;

  QComboBox::wheelEvent(e);
}

const QString AComboBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void AComboBox::setWithoutDisclosures(const QVariant &data) {
  addItem(withoutDisclosures(), data);
  setItemIcon(0, AntiquaApplIcon("dialog-warning"));
}

} // namespace AntiquaCRM
