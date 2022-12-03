// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliveryid.h"

DeliveryId::DeliveryId(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setClearButtonEnabled(false);
  m_edit->setStyleSheet("QLineEdit {background:transparent;border:none;}");
  m_edit->setReadOnly(true);
  m_layout->addWidget(m_edit);
  m_layout->addStretch(1);
  setRequired(false);
  setModified(false);
  connect(m_edit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));
  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void DeliveryId::loadDataset() {}

void DeliveryId::inputChanged(const QString &) { setModified(true); }

void DeliveryId::setValue(const QVariant &val) {
  m_edit->setText(val.toString());
}

void DeliveryId::reset() {
  m_edit->clear();
  setModified(false);
}

void DeliveryId::setFocus() { m_edit->setFocus(); }

void DeliveryId::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  setRequired((field.requiredStatus() == QSqlField::Required));
}

void DeliveryId::setInfo(const QString &info) {
  if (info.isEmpty())
    return;

  QString label(info);
  label.append(":");
  m_label->setVisible(true);
  m_label->setText(label);
  m_edit->setToolTip(info);
}

const QString DeliveryId::info() {
  if (m_label->isVisible())
    return m_edit->toolTip();
  else
    return toolTip();
}

const QVariant DeliveryId::value() { return m_edit->text().trimmed(); }

bool DeliveryId::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

const QString DeliveryId::notes() {
  QString msg(info() + " ");
  msg.append(tr("is required and can't empty."));
  return msg;
}
