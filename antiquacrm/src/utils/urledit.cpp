// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "urledit.h"

#include <QtWidgets/QHBoxLayout>

UrlEdit::UrlEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("UrlEdit");

  setRequired(false);
  setModified(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m_label->setText("Url:");
  layout->addWidget(m_label);

  m_edit = new QLineEdit(this);
  m_edit->setPlaceholderText("https://www.hjcms.de");
  m_edit->setToolTip(tr("Homepage URL"));
  layout->addWidget(m_edit);

  setLayout(layout);

  connect(m_edit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void UrlEdit::inputChanged(const QString &str) {
  Q_UNUSED(str)
  setModified(true);
}

void UrlEdit::setValue(const QVariant &val) {
  p_value = val;
  m_edit->setText(val.toString());
}

void UrlEdit::reset() {
  p_value.clear();
  m_edit->clear();
}

void UrlEdit::setInfo(const QString &info) { m_label->setText(info); }

const QString UrlEdit::info() { return m_label->text(); }

const QVariant UrlEdit::value() { return m_edit->text(); }

bool UrlEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

const QString UrlEdit::notes() { return tr("a Valid Url must edit."); }
