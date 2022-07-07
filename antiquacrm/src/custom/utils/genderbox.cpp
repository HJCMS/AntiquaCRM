// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderbox.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>

Gender::Gender() { p_data = genderData(); }

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

const QString Gender::formOfAddress(const QString &search) {
  if (QObject::tr("Male") == search)
    return QObject::tr("Mr.");

  if (QObject::tr("Female") == search)
    return QObject::tr("Mrs");

  return QString();
}

const QHash<int, QString> Gender::genderData() {
  QHash<int, QString> h;
  h.insert(0, QObject::tr("Without disclosures")); /**< Ohne Angabe */
  h.insert(1, QObject::tr("Male"));                /**< Männlich */
  h.insert(2, QObject::tr("Female"));              /**< Weiblich */
  h.insert(3, QObject::tr("Various"));             /**< Diverse */
  return h;
}

int Gender::indexByString(const QString &search) {
  if (search.contains("Mrs", Qt::CaseInsensitive))
    return 2;
  else if (search.contains("Mr", Qt::CaseInsensitive))
    return 1;
  else if (search.contains("herr", Qt::CaseInsensitive))
    return 1;
  else if (search.contains("frau", Qt::CaseInsensitive))
    return 2;
  else if (search.contains("fräulein", Qt::CaseInsensitive))
    return 2;
  else if (search.contains("diverse", Qt::CaseInsensitive))
    return 3;

  QString buf = search.trimmed();
  QHashIterator<int, QString> it(genderData());
  while (it.hasNext()) {
    it.next();
    if (it.value().contains(buf, Qt::CaseInsensitive))
      return it.key();
  }
  buf.clear();
  return 0;
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
    m_comboBox->insertItem(i, myIcon("group"), gender.value(i));
    QString form = gender.formOfAddress(gender.value(i));
    if (!form.isEmpty()) {
      m_comboBox->setItemData(i, form, Qt::UserRole);
    }
  }

  setModified(false);
  setRequired(false);

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void GenderBox::itemChanged(int) { setModified(true); }

void GenderBox::setValue(const QVariant &val) {
  int i = -1;
  if (val.type() == QVariant::Int) {
    i = val.toInt();
  } else {
    i = m_comboBox->findData(val.toString(), Qt::UserRole);
    if (i < 1)
      i = m_comboBox->findData(val.toString(), Qt::DisplayRole);
  }

  if (i > 0) {
    p_value = i;
    m_comboBox->setCurrentIndex(i);
    setModified(true);
  }
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
