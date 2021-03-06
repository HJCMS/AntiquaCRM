// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "boolbox.h"

#include <QDebug>
#include <QHBoxLayout>

BoolBox::BoolBox(QWidget *parent) : UtilsMain{parent} {
  setWindowTitle(tr("Checkbox"));

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_checkBox = new QCheckBox(this);
  layout->addWidget(m_checkBox);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(itemChanged(int)));
}

void BoolBox::itemChanged(int state) {
  setModified(true);
  switch (state) {
  case Qt::Unchecked:
    emit checked(false);
    break;

  case Qt::Checked:
    emit checked(true);
    break;

  case Qt::PartiallyChecked:
    break;

  default:
    emit checked(false);
  };
}

void BoolBox::setValue(const QVariant &val) {
  bool b = val.toBool();
  m_checkBox->setChecked(b);
  setModified(true);
}

void BoolBox::reset() {
  m_checkBox->setChecked(false);
  setModified(false);
}

void BoolBox::setFocus() { m_checkBox->setFocus(); }

void BoolBox::setChecked(bool b) {
  setModified(true);
  m_checkBox->setChecked(b);
  m_checkBox->setFocus();
}

bool BoolBox::isChecked() { return m_checkBox->isChecked(); }

const QVariant BoolBox::value() { return m_checkBox->isChecked(); }

bool BoolBox::isValid() { return true; }

void BoolBox::setInfo(const QString &txt) { m_checkBox->setText(txt); }

const QString BoolBox::info() { return m_checkBox->text(); }

const QString BoolBox::notes() {
  QString msg(windowTitle());
  msg.append(" ");
  msg.append(tr("is required must selected."));
  return msg;
}
