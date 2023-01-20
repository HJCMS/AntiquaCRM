// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "addressinfo.h"

#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QRegExp>

AddressInfo::AddressInfo(QWidget *parent) : InputEdit{parent} {
  m_edit = new QTextEdit(this);
  m_edit->setToolTip(tr("Address"));
  m_edit->setReadOnly(true);
  m_edit->setAutoFormatting(QTextEdit::AutoNone);
  m_edit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  m_edit->setDocumentTitle(m_edit->toolTip());
  m_edit->setStyleSheet("QTextEdit {background:transparent;border:none;}");
  m_layout->addWidget(m_edit);
  setRequired(false);
  setModified(false);
}

void AddressInfo::loadDataset() {
  QString content = m_edit->toPlainText().trimmed();
  int lb = content.count(QRegExp("\\n"));
  QFontMetrics fm(m_edit->font());
  m_edit->setFixedHeight((fm.height() * (lb + 2)));
}

void AddressInfo::reset() {
  m_edit->clear();
  setModified(false);
}

void AddressInfo::setValue(const QVariant &val) {
  m_edit->setPlainText(val.toString());
  loadDataset();
}

void AddressInfo::setFocus() { m_edit->setFocus(); }

void AddressInfo::setProperties(const QSqlField &field) { Q_UNUSED(field); }

const QVariant AddressInfo::value() { return m_edit->toPlainText(); }

bool AddressInfo::isValid() {
  if (isRequired() && value().toString().isEmpty())
    return false;

  return true;
}

void AddressInfo::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString AddressInfo::info() { return m_edit->toolTip(); }

const QString AddressInfo::notes() { return tr("Address is required."); }
