// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationbox.h"
#include "antiqua_global.h"

#include <QDebug>
// #include <QtGui>
#include <QtWidgets>

static const QStringList salutations() {
  QStringList l;
  l << "Dr.";
  l << "Dr. Dr.";
  l << "Dr. med.";
  l << "Dr. phil.";
  l << "Dr. rer. nat.";
  l << "Dr. iur.";
  l << "Dr. oec. publ.";
  l << "Hofrat";
  l << "Prof.";
  l << "Prof. Dr.";
  l << "Prof. Dr. Dr.";
  l << "Univ. Prof.";
  l << "Dipl. Ing.";
  l << "Dipl.-HTL-Ing.";
  l << "Dipl.-HLFL-Ing.";
  return l;
}

SalutationBox::SalutationBox(QWidget *parent) : UtilsMain{parent} {
  setObjectName("SalutationBox");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_comboBox = new QComboBox(this);
  m_comboBox->setToolTip(tr("Title or Salutation"));
  m_comboBox->insertItem(0, QString());
  m_comboBox->setMinimumWidth(150);
  foreach (QString n, salutations()) {
    m_comboBox->addItem(n);
  }
  layout->addWidget(m_comboBox);

  m_edit = new QLineEdit(this);
  m_edit->setMaxLength(25);
  m_edit->setPlaceholderText(tr("Without disclosures"));
  m_comboBox->setLineEdit(m_edit);

  setRequired(false);
  setModified(false);
  setLayout(layout);

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void SalutationBox::itemChanged(int) { setModified(true); }

void SalutationBox::setValue(const QVariant &v) {
  QString txt = v.toString();
  int index = m_comboBox->findData(v, Qt::DisplayRole, Qt::MatchExactly);
  if (index > 0)
    m_comboBox->setCurrentIndex(index);
  else if (!txt.isEmpty())
    m_edit->setText(txt);
}

void SalutationBox::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

void SalutationBox::setFocus() { m_comboBox->setFocus(); }

const QVariant SalutationBox::value() {
  if (m_comboBox->currentIndex() > 0)
    return m_comboBox->currentText();

  return m_edit->text();
}

bool SalutationBox::isValid() { return true; }

const QString SalutationBox::notes() { return tr("a Title is required"); }
