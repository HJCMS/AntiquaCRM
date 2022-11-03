// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ecurrency.h"

#include <QDebug>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QSize>

Ecurrency::Ecurrency(QWidget *parent) : InputEdit{parent} {
  m_comboBox = new QComboBox(this);
  m_comboBox->setEditable(false);
  loadDataset();
  m_layout->addWidget(m_comboBox);
  QFrame *editFrame = new QFrame(this);
  QHBoxLayout *frameLayout = new QHBoxLayout(editFrame);
  m_lineEdit = new QLineEdit(editFrame);
  m_lineEdit->setMaxLength(3);
  frameLayout->addWidget(m_lineEdit);
  QFontMetrics metric(m_lineEdit->font());
  QSize maxSize = metric.size(Qt::TextSingleLine, "EUR");
  m_lineEdit->setMaximumWidth(maxSize.width());
  editFrame->setLayout(frameLayout);
  m_layout->addWidget(editFrame);
  m_layout->addStretch(1);
  setModified(false);
  setRequired(false);
  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), SLOT(itemChanged(int)));
}

void Ecurrency::loadDataset() {
  int i = 0;
  m_comboBox->insertItem(i++, tr("Without disclosures"), QString());
  m_comboBox->insertItem(i++, QString("€ EUR"), QString("€"));
  m_comboBox->insertItem(i++, QString("$ USD"), QString("$"));
  m_comboBox->insertItem(i++, QString("£ GBP"), QString("£"));
  m_comboBox->insertItem(i++, QString("¥ JPY"), QString("¥"));
  m_comboBox->insertItem(i++, QString("₽ SUR"), QString("₽"));
}

void Ecurrency::itemChanged(int index) {
  m_lineEdit->setText(m_comboBox->itemData(index, Qt::UserRole).toString());
  setModified(true);
}

void Ecurrency::setValue(const QVariant &val) {
  int i = -1;
  if (val.type() == QVariant::String) {
    i = m_comboBox->findData(val.toString(), Qt::UserRole);
  } else if (val.type() == QVariant::Int) {
    i = val.toInt();
  }

  if (i > 0) {
    m_comboBox->setCurrentIndex(i);
    setModified(true);
    return;
  }

  QString data = val.toString();
  if (data.length() > 0 && data.length() < 4) {
    int c = m_comboBox->count();
    m_comboBox->insertItem(++c, data, data);
    setModified(true);
  }
}

void Ecurrency::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

void Ecurrency::setFocus() { m_comboBox->setFocus(); }

void Ecurrency::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant Ecurrency::value() {
  QVariant buffer;
  int index = m_comboBox->currentIndex();
  if (index > 0)
    buffer = m_comboBox->itemData(index, Qt::UserRole);

  return buffer;
}

bool Ecurrency::isValid() {
  int index = m_comboBox->currentIndex();
  if (isRequired() && (index > 0 && !m_lineEdit->text().isEmpty()))
    return true;

  return false;
}

void Ecurrency::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_comboBox->setToolTip("ISO 4217");
  setToolTip(info);
}

const QString Ecurrency::info() { return toolTip(); }

const QString Ecurrency::notes() { return tr("Ecurrency is requierd!"); }
