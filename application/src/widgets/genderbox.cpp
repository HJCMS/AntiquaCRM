// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderbox.h"

#include <QDebug>

GenderBox::GenderBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Gender"));
  m_box->insertItem(0, tr("Without disclosures"), AntiquaCRM::NO_GENDER);
  m_box->insertItem(1, tr("Male"), AntiquaCRM::MALE);
  m_box->insertItem(2, tr("Female"), AntiquaCRM::FEMALE);
  m_box->insertItem(3, tr("Various"), AntiquaCRM::VARIOUS);
  m_box->setCurrentIndex(0);
  m_layout->addWidget(m_box);
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void GenderBox::dataChanged(int) { setModified(true); }

void GenderBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void GenderBox::setValue(const QVariant &val) {
  int index = -1;
  int old = m_box->currentIndex();

  if (val.type() == QVariant::Int) {
    AntiquaCRM::Gender gen = static_cast<AntiquaCRM::Gender>(val.toInt());
    index = m_box->findData(gen, Qt::UserRole);
  } else if (val.type() == QVariant::Double) {
    AntiquaCRM::Gender gen = static_cast<AntiquaCRM::Gender>(val.toInt());
    index = m_box->findData(gen, Qt::UserRole);
  } else {
    index = m_box->findData(val.toString(), Qt::DisplayRole);
  }

  // qDebug() << Q_FUNC_INFO << val << index;

  if (index >= 0)
    m_box->setCurrentIndex(index);

  if (index != old)
    setModified(true);
}

void GenderBox::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void GenderBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QHash<AntiquaCRM::Gender, QString> GenderBox::genderData() {
  QHash<AntiquaCRM::Gender, QString> h;
  h.insert(AntiquaCRM::NO_GENDER, tr("Without disclosures"));
  h.insert(AntiquaCRM::MALE, tr("Male"));
  h.insert(AntiquaCRM::FEMALE, tr("Female"));
  h.insert(AntiquaCRM::VARIOUS, tr("Various"));
  return h;
}

const QVariant GenderBox::value() {
  int g = m_box->itemData(m_box->currentIndex(), Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::Gender>(g);
}

bool GenderBox::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void GenderBox::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString GenderBox::info() { return m_box->toolTip(); }

const QString GenderBox::notes() { return tr("This Input is required."); }
