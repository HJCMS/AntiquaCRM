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
  view()->setAlternatingRowColors(true);
  view()->setSelectionMode(QAbstractItemView::SingleSelection);
}

void AComboBox::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QComboBox::wheelEvent(e);
  }
}

const QString AComboBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void AComboBox::setWithoutDisclosures(const QVariant &data) {
  addItem(withoutDisclosures(), data);
  setItemIcon(0, AntiquaApplIcon("dialog-warning"));
}

} // namespace AntiquaCRM
