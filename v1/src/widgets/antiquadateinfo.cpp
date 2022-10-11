// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquadateinfo.h"

#include <QDateTime>
#include <QDebug>

AntiquaDateInfo::AntiquaDateInfo(QWidget *parent) : InputEdit{parent} {
  m_edit = new QDateTimeEdit(this);
  m_edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_edit->setDisplayFormat(ANTIQUACRM_DATETIME_DISPLAY);
  m_edit->setInputMethodHints(Qt::ImhNone);
  m_edit->setDateTime(QDateTime::currentDateTime());
  m_edit->setReadOnly(true);
  QString tip(m_edit->dateTime().toString(ANTIQUACRM_DATETIME_TOOLTIP));
  m_edit->setToolTip(tip);
  m_layout->addWidget(m_edit);
  setRequired(false);
  setStyleSheet("QDateTimeEdit {border:none;background:transparent;}");
}

void AntiquaDateInfo::loadDataset() {}

void AntiquaDateInfo::dataChanged(int) {}

void AntiquaDateInfo::reset() {
  m_edit->setDateTime(QDateTime::currentDateTime());
  setModified(false);
}

void AntiquaDateInfo::setValue(const QVariant &val) {
  if (val.type() == QVariant::Date) {
    m_edit->setDate(val.toDate());
  } else if (val.type() == QVariant::DateTime) {
    m_edit->setDateTime(val.toDateTime());
  } else if (val.type() == QVariant::String) {
    QString str = val.toString().trimmed();
    QDateTime dt = QDateTime::fromString(str, ANTIQUACRM_TIMESTAMP_IMPORT);
    if (dt.isValid())
      m_edit->setDateTime(dt);
  }
}

void AntiquaDateInfo::setFocus() {}

const QVariant AntiquaDateInfo::value() { return m_edit->dateTime(); }

bool AntiquaDateInfo::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void AntiquaDateInfo::setInfo(const QString &info) {
  setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString AntiquaDateInfo::info() { return toolTip(); }

const QString AntiquaDateInfo::notes() {
  return tr("A Valid Date is required!");
}
