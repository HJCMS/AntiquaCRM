// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "vatselecter.h"
#include "applsettings.h"

#include <QLayout>

VATSelecter::VATSelecter(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  ApplSettings cfg(this);
  int vat1 = cfg.value("payment/vat1").toInt();
  int vat2 = cfg.value("payment/vat2").toInt();
  m_box = new QComboBox(this);
  m_box->insertItem(0, QString::number(vat2) + "% " + tr("Reduced"));
  m_box->setItemData(0, vat2, Qt::UserRole);
  m_box->insertItem(1, QString::number(vat1) + "% " + tr("Normal"));
  m_box->setItemData(1, vat1, Qt::UserRole);
  m_box->insertItem(2, tr("No VAT"));
  m_box->setItemData(2, 0, Qt::UserRole);
  m_box->setCurrentIndex(0);
  layout->addWidget(m_box);
  layout->addStretch(1);
  setLayout(layout);
  setModified(false);
  setRequired(true);
}

void VATSelecter::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void VATSelecter::setFocus() { m_box->setFocus(); }

void VATSelecter::setValue(const QVariant &val) {
  int _vat = val.toInt();
  for (int i = 0; i < m_box->count(); i++) {
    if (_vat == m_box->itemData(i, Qt::UserRole).toInt()) {
      m_box->setCurrentIndex(i);
      setModified(true);
      break;
    }
  }
}

const QVariant VATSelecter::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toInt();
}

bool VATSelecter::isValid() { return true; }

void VATSelecter::setInfo(const QString &info) { m_box->setToolTip(info); }

const QString VATSelecter::info() { return m_box->toolTip(); }

const QString VATSelecter::notes() { return tr("A Valid VAT must set."); }
