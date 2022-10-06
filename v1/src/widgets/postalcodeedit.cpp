// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodeedit.h"
#include "postalcodemodel.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QJsonObject>

PostalCodeEdit::PostalCodeEdit(QWidget *parent) : InputEdit{parent} {
  m_label = new QLabel(this);
  m_layout->addWidget(m_label);

  m_countries = new QComboBox(this);
  m_countries->setToolTip(tr("Supported countries"));
  m_countries->insertItem(0, tr("Without disclosures"), QString());
  m_layout->addWidget(m_countries);

  m_postalcode = new QLineEdit(this);
  m_postalcode->setMaxLength(12);
  m_postalcode->setMaximumWidth(100);
  m_postalcode->setPlaceholderText(tr("Postalcode"));
  m_layout->addWidget(m_postalcode);

  setTabOrder(m_countries, m_postalcode);
  loadDataset();

  connect(m_countries, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));

  connect(m_postalcode, SIGNAL(editingFinished()), this,
          SIGNAL(sendLeaveEditor()));
}

void PostalCodeEdit::loadDataset() {
  m_completer = new QCompleter(m_postalcode);
  m_completer->setMaxVisibleItems(20);

  m_completerModel = new PostalCodeModel(m_completer);
  int i = m_countries->count();
  foreach (QString c, m_completerModel->keys()) {
    m_countries->insertItem(++i, c, c);
  }

  m_completer->setModel(m_completerModel);
  m_postalcode->setCompleter(m_completer);
}

void PostalCodeEdit::dataChanged(int index) {
  QString country = m_countries->itemData(index, Qt::UserRole).toString();
  if (country.isEmpty())
    return;

  qDebug() << Q_FUNC_INFO << country;
}

void PostalCodeEdit::reset() {
  m_postalcode->clear();
  if (m_countries->currentIndex() != 0)
    m_countries->setCurrentIndex(0);

  setModified(false);
}

void PostalCodeEdit::setValue(const QVariant &) {}

void PostalCodeEdit::setFocus() { m_postalcode->setFocus(); }

const QVariant PostalCodeEdit::value() {
  QString buffer = m_postalcode->text();
  return buffer.trimmed();
}

bool PostalCodeEdit::isValid() {
  if (isRequired() && m_postalcode->text().isEmpty())
    return false;

  return true;
}

void PostalCodeEdit::setInfo(const QString &info) {
  m_postalcode->setToolTip(info);
  m_label->setText(info + ":");
}

const QString PostalCodeEdit::info() { return toolTip(); }

const QString PostalCodeEdit::notes() {
  return tr("a valid Postalcode is required!");
}
