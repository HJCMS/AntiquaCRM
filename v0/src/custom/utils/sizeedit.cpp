// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sizeedit.h"

#include <QHBoxLayout>
#include <QSize>
#include <QSizePolicy>

SizeEdit::SizeEdit(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QSizePolicy sp(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->insertWidget(0, m_info);

  m_width = new QSpinBox(this);
  m_width->setRange(0, 9999);
  m_width->setSizePolicy(sp);
  m_width->setMinimumWidth(30);
  layout->insertWidget(1, m_width);

  QLabel *middle = new QLabel("x", this);
  middle->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
  layout->insertWidget(2, middle);

  m_height = new QSpinBox(this);
  m_height->setRange(0, 9999);
  m_height->setSizePolicy(sp);
  m_height->setMinimumWidth(30);
  layout->insertWidget(3, m_height);

  layout->addStretch(1);
  layout->setStretch(0, 1);
  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_width, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged(int)));
  connect(m_height, SIGNAL(valueChanged(int)), this, SLOT(sizeChanged(int)));
}

void SizeEdit::sizeChanged(int) { setModified(true); }

void SizeEdit::reset() {
  m_width->setValue(0);
  m_height->setValue(0);
  setModified(false);
}

void SizeEdit::setFocus() { m_width->setFocus(); }

void SizeEdit::setValue(const QVariant &val) {
  QSize s = val.toSize();
  m_width->setValue(s.width());
  m_height->setValue(s.height());
  setModified(true);
}

const QVariant SizeEdit::value() {
  return QSize(m_width->value(), m_height->value());
}

void SizeEdit::setMinimum(int width, int height) {
  m_width->setMinimum(width);
  m_height->setMinimum(height);
}

void SizeEdit::setMaximum(int width, int height) {
  m_width->setMaximum(width);
  m_height->setMaximum(height);
}

bool SizeEdit::isValid() {
  if (isRequired() && (m_width->value() < 1 || m_height->value() < 1))
    return false;

  return true;
}

void SizeEdit::setInfo(const QString &info) {
  QString txt(":");
  txt.prepend(info);
  m_info->setText(txt);
  setToolTip(info);
}

const QString SizeEdit::info() { return toolTip(); }

const QString SizeEdit::notes() {
  QString msg = info().isEmpty() ? tr("Sizing") : info();
  msg.append(" ");
  msg.append(tr("is required must selected."));
  return msg;
}
