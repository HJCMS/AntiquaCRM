// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "boolbox.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

BoolBox::BoolBox(QWidget *parent) : UtilsMain{parent} {
  setWindowTitle(tr("Checkbox"));

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_checkBox = new QCheckBox(this);
  layout->addWidget(m_checkBox);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(itemChanged(int)));
}

void BoolBox::itemChanged(int) { setModified(true); }

void BoolBox::setValue(const QVariant &val) {
  bool b = val.toBool();
  m_checkBox->setChecked(b);
  setModified(true);
}

void BoolBox::reset() {
  m_checkBox->setChecked(false);
  setModified(false);
}

void BoolBox::setChecked(bool b) { m_checkBox->setChecked(b); }

bool BoolBox::isChecked() { return m_checkBox->isChecked(); }

const QVariant BoolBox::value() { return QVariant(m_checkBox->isChecked()); }

bool BoolBox::isValid() {
  if (isRequired())
    return m_checkBox->isChecked();

  return true;
}

void BoolBox::setInfo(const QString &txt) { m_checkBox->setText(txt); }

const QString BoolBox::info() { return m_checkBox->text(); }

const QString BoolBox::notes() {
  QString msg(windowTitle());
  msg.append(" ");
  msg.append(tr("is required must selected."));
  return msg;
}
