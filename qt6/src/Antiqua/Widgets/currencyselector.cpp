// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "currencyselector.h"

#include <QDebug>
#include <QLineEdit>

namespace AntiquaCRM {

CurrencySelector::CurrencySelector(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent}, QLocale{QLocale::system()} {
  m_box = new AntiquaCRM::AComboBox(this);
  m_edit = new ALineEdit(m_box);
  m_edit->setClearButtonEnabled(false);
  m_edit->setPlaceholderText(m_box->withoutDisclosures());
  m_box->setLineEdit(m_edit);
  m_box->view()->setAlternatingRowColors(true);
  layout->addWidget(m_box);

  initData();
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void CurrencySelector::valueChanged(int) { setWindowModified(true); }

void CurrencySelector::initData() {
  m_box->clear();
  QMapIterator<QLocale::Territory, QString> it(cuerrencies());
  while (it.hasNext()) {
    it.next();
    QLocale _lc(QLocale::AnyLanguage, it.key());
    QString _s = _lc.currencySymbol(QLocale::CurrencySymbol);
    QString _n = _lc.currencySymbol(QLocale::CurrencyIsoCode);
    QString _d(_s + " - " + it.value() + " - " + _n);
    m_box->addItem(_d.trimmed(), _s);
  }
  int index = m_box->findData(currencySymbol(QLocale::CurrencySymbol),
                              Qt::UserRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);

  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);

  setWindowModified(false);
}

void CurrencySelector::setValue(const QVariant &value) {
  int index = m_box->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);
  else
    m_edit->setText(value.toString());
}

void CurrencySelector::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void CurrencySelector::reset() {
  m_box->setCurrentIndex(0);
  // The last line
  setWindowModified(false);
}

void CurrencySelector::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void CurrencySelector::setInputToolTip(const QString &tip) {
  m_box->setToolTip(tip);
}

void CurrencySelector::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_box);
}

bool CurrencySelector::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QVariant CurrencySelector::getValue() { return m_edit->text().trimmed(); }

const QString CurrencySelector::popUpHints() {
  // MessageBox Notifications
  return tr("__TODO__");
}

const QString CurrencySelector::statusHints() {
  // StatusBar Notifications
  return tr("__TODO__");
}

const QMap<QLocale::Territory, QString> CurrencySelector::cuerrencies() {
  QMap<QLocale::Territory, QString> _m;
  _m.insert(QLocale(QLocale::German, QLocale::Europe).territory(),
            tr("European Union"));
  _m.insert(QLocale::Switzerland, tr("Switzerland"));
  _m.insert(QLocale::UnitedKingdom, tr("United Kingdom"));
  _m.insert(QLocale::UnitedStates, tr("United States"));
  _m.insert(QLocale::China, tr("China"));
  _m.insert(QLocale::Japan, tr("Japan"));
  _m.insert(QLocale::India, tr("India"));
  _m.insert(QLocale::Australia, tr("Australia"));
  _m.insert(QLocale::Brazil, tr("Brazil"));
  _m.insert(QLocale::SouthAfrica, tr("South Africa"));
  _m.insert(QLocale::SouthKorea, tr("South Korea"));
  return _m;
}

} // namespace AntiquaCRM
