// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearselect.h"

#include <QIcon>

YearSelect::YearSelect(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Select Year"));
  m_layout->addWidget(m_box);
  btn_open = new QPushButton(tr("Open"), this);
  btn_open->setIcon(QIcon("://icons/action_search.png"));
  m_layout->addWidget(btn_open);
  m_layout->addStretch(1);

  loadDataset();

  QString title(tr("Open Year"));
  title.append(" ");
  title.append(m_box->currentText());
  btn_open->setText(title);

  setRequired(false);
  setModified(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
  connect(btn_open, SIGNAL(clicked()), SIGNAL(sendYearOpen()));
}

void YearSelect::loadDataset() {
  QDate from_year = QDate::currentDate().addYears(-factor);
  for (int i = 0; i <= factor; ++i) {
    QDate y_date = from_year.addYears(i);
    QString str = QString::number(y_date.year());
    m_box->addItem(str, y_date);
  }
}

void YearSelect::dataChanged(int index) {
  p_curDate = m_box->itemData(index, Qt::UserRole).toDate();
  if (!p_curDate.isValid())
    return;

  QString title(tr("Open Year"));
  title.append(" ");
  title.append(m_box->currentText());
  btn_open->setText(title);
}

void YearSelect::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void YearSelect::setValue(const QVariant &val) {
  QDate date = val.toDate();
  if (date.isValid()) {
    int index = m_box->findData(date, Qt::UserRole);
    if (index >= 0)
      m_box->setCurrentIndex(index);
  }
}

void YearSelect::setFocus() { m_box->setFocus(); }

void YearSelect::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant YearSelect::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toDate();
}

bool YearSelect::isValid() {
  if (isRequired() && value().toDate().isValid())
    return false;

  return true;
}

void YearSelect::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString YearSelect::info() { return m_box->toolTip(); }

const QString YearSelect::notes() { return tr("Invalid Year selection."); }
