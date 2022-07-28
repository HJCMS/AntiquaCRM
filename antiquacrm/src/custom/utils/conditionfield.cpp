// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "conditionfield.h"

#include <QHBoxLayout>

ConditionBox::ConditionBox(QWidget *parent) : QComboBox{parent} {
  /**
   * Ohne Angabe
   */
  insertItem(0, tr("Without disclosures"));
  /**
   * ABEBOOKS: FINE BOOK CONDITION
   * BOOKLOOKER: Sehr gut, fast neuwertig!
   */
  insertItem(1, "1) " + tr("Very good, almost new!") + " (" + tr("Very good") + ")");
  /**
   * ABEBOOKS: VERY GOOD BOOK CONDITION
   * BOOKLOOKER: Leichte Gebrauchsspuren
   */
  insertItem(2, "2) " + tr("Slight signs of use") + " (" + tr("Good") + ")");
  /**
   * ABEBOOKS: GOOD BOOK CONDITION
   * BOOKLOOKER: Deutliche Gebrauchsspuren
   */
  insertItem(3, "3) " + tr("Clear signs of use") + " (" + tr("Satisfying") + ")");
  /**
   * ABEBOOKS: FAIR BOOK CONDITION
   * BOOKLOOKER: Stark abgenutzt
   */
  insertItem(4, "4) " + tr("Heavily worn") + " (" + tr("Sufficient") + ")");
}

ConditionField::ConditionField(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  m_box = new ConditionBox(this);
  m_box->setEditable(false);
  m_box->setToolTip(tr("Book Condition"));
  layout->addWidget(m_box);
  setLayout(layout);
  setModified(false);
  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void ConditionField::dataChanged(int) { setModified(true); }

void ConditionField::setValue(const QVariant &val) {
  int id = val.toInt();
  if (id > 0)
    m_box->setCurrentIndex(id);
}

void ConditionField::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void ConditionField::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void ConditionField::setInfo(const QString &info) { m_box->setToolTip(info); }

const QString ConditionField::info() { return m_box->toolTip(); }

const QVariant ConditionField::value() { return m_box->currentIndex(); }

bool ConditionField::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

const QString ConditionField::notes() { return tr("Condition must set!"); }
