// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacombobox.h"
#include <ASettings>
#include <QIcon>

AntiquaComboBox::AntiquaComboBox(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setInsertPolicy(QComboBox::NoInsert);
  setEditable(false);

  AntiquaCRM::ASettings cfg(this);
  mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();

  m_view = view();
  if (m_view != nullptr) {
    m_view->setAlternatingRowColors(true);
    m_view->setSelectionMode(QAbstractItemView::SingleSelection);
  }
}

void AntiquaComboBox::wheelEvent(QWheelEvent *e) {
  if (mouseWheel) {
    QComboBox::wheelEvent(e);
  }
}

const QString AntiquaComboBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void AntiquaComboBox::setWithoutDisclosures(const QVariant &data) {
  addItem(withoutDisclosures(), data);
  setItemIcon(0, QIcon("://icons/warning.png"));
}

void AntiquaComboBox::setMouseWheelEnabled(bool b) {
  mouseWheel = b;
  emit sendMouseWheelChanged();
}

bool AntiquaComboBox::isMouseWheelEnabled() { return mouseWheel; }
