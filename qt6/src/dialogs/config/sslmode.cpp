// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sslmode.h"

SslMode::SslMode(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setWithoutDisclosures();
  layout->addWidget(m_edit);
  initData();

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SslMode::initData() {
  int _i = m_edit->count();
  m_edit->insertItem(_i++, tr("Prefer"), QString("prefer"));
  m_edit->insertItem(_i++, tr("Required"), QString("require"));
  m_edit->insertItem(_i++, tr("Verify CA"), QString("verify-ca"));
  m_edit->insertItem(_i++, tr("Verify full"), QString("verify-full"));
}

void SslMode::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SslMode::setValue(const QVariant &value) {
  int _index = m_edit->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SslMode::setFocus() { m_edit->setFocus(); }

void SslMode::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SslMode::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SslMode::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void SslMode::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  AntiquaCRM::ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SslMode::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SslMode::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SslMode::getValue() {
  return m_edit->itemData(m_edit->currentIndex(), Qt::UserRole);
}

const QString SslMode::popUpHints() {
  return tr("A selected CA Mode is missing.");
}

const QString SslMode::statusHints() { return popUpHints(); }
