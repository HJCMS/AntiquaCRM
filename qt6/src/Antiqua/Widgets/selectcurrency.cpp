// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectcurrency.h"

#include <QDebug>
#include <QLineEdit>

namespace AntiquaCRM {

SelectCurrency::SelectCurrency(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent}, QLocale{QLocale::system()} {
  m_box = new AntiquaCRM::AComboBox(this);
  m_edit = new ALineEdit(m_box);
  m_edit->setClearButtonEnabled(false);
  m_edit->setPlaceholderText(m_box->withoutDisclosures());
  m_box->setLineEdit(m_edit);
  m_box->view()->setAlternatingRowColors(true);
  layout->addWidget(m_box);

  // Important and required
  m_box->addItem(QString(), QString());

  QMapIterator<QLocale::Territory, QString> it(cuerrencies());
  while (it.hasNext()) {
    it.next();
    QLocale _lc(QLocale::AnyLanguage, it.key());
    QString _s = _lc.currencySymbol(QLocale::CurrencySymbol);
    QString _n = _lc.currencySymbol(QLocale::CurrencyIsoCode);
    QString _d(_s + " - " + it.value() + " - " + _n);
    m_box->addItem(_d.trimmed(), _s);
  }

  m_view = new ALabel(this);
  m_view->setMinimumWidth(20);
  layout->addWidget(m_view);

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(currencySelected(int)));
  connect(m_edit, SIGNAL(returnPressed()), SLOT(currencyEdited()));

  initData();
}

bool SelectCurrency::checkSymbol(const QString &symbol) const {
  const QString _s(symbol.trimmed());
  if (_s.length() == 1)
    return true;
  else if (_s.length() > 1 && !_s.contains(" "))
    return true;

  return false;
}

void SelectCurrency::currencySelected(int) {
  if (m_box->currentIndex() == 0)
    return;

  m_view->setText(getValue().toString());
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectCurrency::currencyEdited() {
  QString _str = m_edit->text().trimmed();
  if (!checkSymbol(_str))
    return;

  m_view->setText(_str);
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectCurrency::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(currencySymbol(QLocale::CurrencySymbol));
  setRestrictions(_f);

  int index = m_box->findData(currencySymbol(QLocale::CurrencySymbol),
                              Qt::UserRole, Qt::MatchExactly);
  if (index > 0)
    m_box->setCurrentIndex(index);

  setWindowModified(false);
}

void SelectCurrency::setValue(const QVariant &value) {
  int _index = m_box->findData(value, Qt::UserRole, Qt::MatchExactly);
  if (_index > 0) {
    m_edit->clear();
    m_box->setCurrentIndex(_index);
    return;
  }

  m_edit->setText(value.toString());
}

void SelectCurrency::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void SelectCurrency::reset() {
  m_edit->clear();
  m_box->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectCurrency::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SelectCurrency::setInputToolTip(const QString &tip) {
  m_box->setToolTip(tip);
}

void SelectCurrency::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_box);
}

bool SelectCurrency::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QMetaType SelectCurrency::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SelectCurrency::getValue() {
  int _index = m_box->currentIndex();
  if (_index > 0) {
    QString _symbol = m_box->itemData(_index, Qt::UserRole).toString();
    if (checkSymbol(_symbol))
      return _symbol;
  }

  const QString _edit = m_edit->text().trimmed();
  if (_index == 0 && checkSymbol(_edit))
    return _edit;

  qWarning("invalid currency symbol detected - fallback!");
  return currencySymbol(QLocale::CurrencySymbol);
}

const QString SelectCurrency::popUpHints() {
  return tr("A valid currency is required for this entry.");
}

const QString SelectCurrency::statusHints() {
  return tr("Please enter a valid currency.");
}

const QMap<QLocale::Territory, QString> SelectCurrency::cuerrencies() {
  QMap<QLocale::Territory, QString> _m;
  _m.insert(QLocale::France, tr("European Union"));
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
