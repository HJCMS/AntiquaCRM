// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderbox.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

Gender::Gender() {
  p_data.insert(0, QObject::tr("Without disclosures")); /**< Ohne Angabe */
  p_data.insert(1, QObject::tr("Male"));                /**< Männlich */
  p_data.insert(2, QObject::tr("Female"));              /**< Weiblich */
  p_data.insert(3, QObject::tr("Various"));             /**< Diverse */
}

int Gender::size() { return p_data.size(); }

const QString Gender::value(int index) { return p_data.value(index); }

const QStringList Gender::all() {
  QStringList list;
  QHash<int, QString>::iterator i;
  for (i = p_data.begin(); i != p_data.end(); ++i) {
    list.append(i.value());
  }
  return list;
}

GenderBox::GenderBox(QWidget *parent) : UtilsMain{parent} {
  setObjectName("GenderBox");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_comboBox = new QComboBox(this);
  m_comboBox->setEditable(false);
  layout->addWidget(m_comboBox);

  setLayout(layout);

  Gender gender;
  for (int i = 0; i < gender.size(); i++) {
    m_comboBox->addItem(myIcon("group"), gender.value(i));
  }

  setModified(false);
  setRequired(false);

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void GenderBox::itemChanged(int) { setModified(true); }

void GenderBox::setValue(const QVariant &i) {
  m_comboBox->setCurrentIndex(i.toInt());
}

void GenderBox::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}
void GenderBox::setFocus() { m_comboBox->setFocus(); }

const QVariant GenderBox::value() { return m_comboBox->currentIndex(); }

bool GenderBox::isValid() { return true; }

void GenderBox::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_info->setText(txt);
  m_comboBox->setToolTip(info);
}

const QString GenderBox::info() { return m_info->text(); }

const QString GenderBox::notes() { return QString(); }
