// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ecurrency.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QLocale>
#include <QSize>

Ecurrency::Ecurrency(QWidget *parent) : InputEdit{parent} {
  QLocale lc = QLocale::system();
  p_defaultSymbol = QLocale::system().currencySymbol();
  p_defaultISO4217 = lc.currencySymbol(QLocale::CurrencyIsoCode);

  m_box = new AntiquaComboBox(this);
  m_box->setEditable(false);
  m_layout->addWidget(m_box);
  QFrame *editFrame = new QFrame(this);
  QHBoxLayout *frameLayout = new QHBoxLayout(editFrame);
  m_lineEdit = new QLineEdit(editFrame);
  m_lineEdit->setMaxLength(3);
  frameLayout->addWidget(m_lineEdit);
  QFontMetrics metric(m_lineEdit->font());
  QSize maxSize = metric.size(Qt::TextSingleLine, p_defaultISO4217);
  m_lineEdit->setMaximumWidth(maxSize.width());
  editFrame->setLayout(frameLayout);
  m_layout->addWidget(editFrame);
  m_layout->addStretch(1);

  loadDataset();
  setModified(false);
  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(itemChanged(int)));
}

void Ecurrency::loadDataset() {
  int i = 0;
  m_box->insertItem(i++, m_box->withoutDisclosures(), QString());
  m_box->insertItem(i++, QString("€ EUR"), QString("€"));
  m_box->insertItem(i++, QString("$ USD"), QString("$"));
  m_box->insertItem(i++, QString("£ GBP"), QString("£"));
  m_box->insertItem(i++, QString("¥ JPY"), QString("¥"));
  m_box->insertItem(i++, QString("₽ SUR"), QString("₽"));
  m_lineEdit->setText(p_defaultSymbol);
}

void Ecurrency::itemChanged(int index) {
  m_lineEdit->setText(m_box->itemData(index, Qt::UserRole).toString());
  setModified(true);
}

void Ecurrency::setValue(const QVariant &val) {
  int i = -1;
  if (val.type() == QVariant::String) {
    i = m_box->findData(val.toString(), Qt::UserRole);
  } else if (val.type() == QVariant::Int) {
    i = val.toInt();
  }

  if (i > 0) {
    m_box->setCurrentIndex(i);
    setModified(true);
    return;
  }

  QString data = val.toString();
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << data;
#endif
  if (data.length() > 0 && data.length() < 4) {
    int c = m_box->count();
    m_box->insertItem(++c, data, data);
    setModified(true);
  }
}

void Ecurrency::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void Ecurrency::setFocus() { m_box->setFocus(); }

void Ecurrency::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant Ecurrency::value() {
  QVariant buffer;
  int index = m_box->currentIndex();
  if (index > 0)
    buffer = m_box->itemData(index, Qt::UserRole);

  if (buffer.isNull())
    return QLocale::system().currencySymbol(QLocale::CurrencySymbol);

  return buffer.toString();
}

bool Ecurrency::isValid() {
  int index = m_box->currentIndex();
  if (isRequired() && (index > 0 && !m_lineEdit->text().isEmpty()))
    return true;

  return false;
}

void Ecurrency::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_box->setToolTip("ISO 4217");
  setToolTip(info);
}

const QString Ecurrency::info() { return toolTip(); }

const QString Ecurrency::notes() { return tr("A valid Currency is Required!"); }
