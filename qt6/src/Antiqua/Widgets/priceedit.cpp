// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "priceedit.h"

#include <QDebug>
#include <QLabel>
#include <QLocale>

namespace AntiquaCRM {

constexpr qint64 price_maximum = 999999999;

PriceEdit::PriceEdit(double min, double max, QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ADoubleBox(this);
  m_edit->setMinimum(min);
  m_edit->setMaximum(max);
  m_edit->setSuffix(" " + currencySymbol());
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(valueChanged(double)), SLOT(valueChanged(double)));
}

PriceEdit::PriceEdit(QWidget *parent) : PriceEdit{0, price_maximum, parent} {}

bool PriceEdit::fromMoneyString(const QString &money) const {
  QString _buf = money.trimmed();
  if (_buf.count(",") != 1)
    return false; // not a fixable money string

  _buf.replace(currencySymbol(), "");
  _buf.replace(",", ".");
  _buf.replace(" ", "");

  bool _b;
  double _d = _buf.trimmed().toDouble(&_b);
  if (_b && (_d > m_edit->minimum())) {
    m_edit->setValue(_d);
    return true;
  }
  return false;
}

void PriceEdit::valueChanged(double d) {
  if (d < m_edit->minimum())
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void PriceEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0.00);
  setRestrictions(_f);
  setWindowModified(false);
}

void PriceEdit::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
  case (QMetaType::Double):
    m_edit->setValue(value.toDouble());
    break;

  case (QMetaType::QString): {
    if (!fromMoneyString(value.toString()))
      qWarning("Invalid given Data Type in PriceEdit.");
  } break;

  default:
    qWarning("Invalid given Data Type in PriceEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "PriceEdit Requires type double but get:" << value;
#endif
    break;
  };
}

void PriceEdit::setFocus() { m_edit->setFocus(); }

void PriceEdit::reset() {
  m_edit->setValue(m_edit->minimum());
  setWindowModified(false);
}

void PriceEdit::updateCurrencySuffix(const QString &symbol) {
  m_edit->setSuffix(" " + symbol);
}

void PriceEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() != QSqlField::Required) {
    setRequired(false);
    return;
  }

  double _current = m_edit->value();
  double _default = field.defaultValue().toDouble();
  if ((_default != _current) && (_current <= m_edit->minimum())) {
    m_edit->setValue(_default);
  }
  setRequired(true);
}

void PriceEdit::setMinimum(double min) { m_edit->setMinimum(min); }

void PriceEdit::setMaximum(double max) { m_edit->setMaximum(max); }

void PriceEdit::setRange(double min, double max) { m_edit->setRange(min, max); }

void PriceEdit::setSingleStep(double steps) {
  if (steps < 0.01)
    return;

  m_edit->setSingleStep(steps);
}

void PriceEdit::setInputToolTip(const QString &str) { m_edit->setToolTip(str); }

void PriceEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool PriceEdit::isValid() {
  if (isRequired() && (m_edit->value() <= m_edit->minimum()))
    return false;

  if (m_edit->value() <= m_edit->minimum())
    return false;

  return true;
}

const QString PriceEdit::currencySymbol() const {
  QString sym = QLocale::system().currencySymbol(QLocale::CurrencySymbol);
  return config->value("payment/currency", sym).toString();
}

const QMetaType PriceEdit::getType() const {
  return QMetaType(QMetaType::Double);
}

const QVariant PriceEdit::getValue() { return m_edit->value(); }

const QString PriceEdit::getMoney() const {
  QLocale _locale = QLocale::system();
  QString _symbol = _locale.currencySymbol(QLocale::CurrencySymbol);
  _symbol = (_symbol == m_edit->suffix()) ? _symbol : m_edit->suffix();
  return _locale.toCurrencyString(m_edit->value(), _symbol, 2);
}

const QString PriceEdit::popUpHints() {
  QStringList info;
  info << tr("This entry requires a valid price!");
  info << tr("The minimum value specified in the configuration has been fallen "
             "below or the entry is not a valid price!");
  return info.join("\n");
}

const QString PriceEdit::statusHints() {
  return tr("Price is invalid or empty!");
}

} // namespace AntiquaCRM
