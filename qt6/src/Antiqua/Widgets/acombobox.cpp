// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acombobox.h"

#include <QAbstractItemView>
#include <QIcon>
#include <QMetaType>

namespace AntiquaCRM {

AComboBox::AComboBox(QWidget *parent) : QComboBox{parent}, AInputEdit{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setInsertPolicy(QComboBox::NoInsert);
  setEditable(false);
  mouse_wheel_support = behaviorSettings("mouse_wheel_actions");
  if (view() != nullptr) {
    view()->setAlternatingRowColors(true);
    view()->setSelectionMode(QAbstractItemView::SingleSelection);
  }
}

void AComboBox::wheelEvent(QWheelEvent *e) {
  if (mouse_wheel_support) {
    QComboBox::wheelEvent(e);
  }
}

void AComboBox::setValue(const QVariant &value) {
  int index = findData(value, Qt::UserRole);
  if (index >= 0)
    setCurrentIndex(index);
}

void AComboBox::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QString AComboBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void AComboBox::setWithoutDisclosures(const QVariant &data) {
  addItem(withoutDisclosures(), data);
  setItemIcon(0, QIcon("://icons/warning.png"));
}

const QVariant AComboBox::getValue() {
  return itemData(currentIndex(), Qt::UserRole);
}

} // namespace AntiquaCRM
