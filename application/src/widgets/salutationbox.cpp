// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationbox.h"

SalutationBox::SalutationBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Salutation"));
  m_layout->addWidget(m_box);
  loadDataset();

  m_edit = new QLineEdit(this);
  m_edit->setMaxLength(25);
  m_edit->setPlaceholderText(withoutDisclosures());
  m_box->setLineEdit(m_edit);

  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

const QString SalutationBox::withoutDisclosures() {
  return tr("Without disclosures");
}

void SalutationBox::loadDataset() {
  m_box->clear();
  m_box->addItem(QString(), QString());
  foreach (QString s, salutations()) {
    m_box->addItem(s, s);
  }
}

void SalutationBox::dataChanged(int) { setModified(true); }

void SalutationBox::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void SalutationBox::setValue(const QVariant &val) {
  QString txt = val.toString();
  int index = m_box->findData(val, Qt::DisplayRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);
  else if (!txt.isEmpty())
    m_edit->setText(txt);
}

void SalutationBox::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void SalutationBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant SalutationBox::value() {
  if (m_box->currentIndex() > 0)
    return m_box->currentText();

  QString edit = m_edit->text().trimmed();
  if (edit != withoutDisclosures())
    return edit;

  return QString();
}

bool SalutationBox::isValid() { return true; }

void SalutationBox::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString SalutationBox::info() { return m_box->toolTip(); }

const QString SalutationBox::notes() { return tr("a Title is required."); }

const QStringList SalutationBox::salutations() {
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
