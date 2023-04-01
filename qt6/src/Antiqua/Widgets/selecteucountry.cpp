// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecteucountry.h"

namespace AntiquaCRM {

AEUCountries::AEUCountries(QObject *parent)
    : QObject{parent}, QMap<QString, QString>{} {
  insert("XX", tr("Non European Country"));
  insert("BE", tr("Belgium"));
  insert("BG", tr("Bulgaria"));
  insert("DK", tr("Denmark"));
  insert("DE", tr("Germany"));
  insert("EE", tr("Estonia"));
  insert("FI", tr("Finland"));
  insert("FR", tr("France"));
  insert("GR", tr("Greece"));
  insert("IE", tr("Ireland"));
  insert("IT", tr("Italy"));
  insert("HR", tr("Croatia"));
  insert("LV", tr("Latvia"));
  insert("LT", tr("Lithuania"));
  insert("LU", tr("Luxembourg"));
  insert("MT", tr("Malta"));
  insert("NL", tr("Netherlands"));
  insert("AT", tr("Austria"));
  insert("PL", tr("Poland"));
  insert("PT", tr("Portugal"));
  insert("RO", tr("Romania"));
  insert("SE", tr("Sweden"));
  insert("SK", tr("Slovakia"));
  insert("SI", tr("Slovenia"));
  insert("ES", tr("Spain"));
  insert("CZ", tr("Czech"));
  insert("HU", tr("Hungary"));
  insert("CY", tr("Cyprus"));
}

const QString AEUCountries::bcp47Name(const QString &country) {
  return key(country, "XX");
}

const QString AEUCountries::country(const QString &bcp47) {
  return contains(bcp47) ? value(bcp47) : QString();
}

SelectEUCountry::SelectEUCountry(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectEUCountry::valueChanged(int) {
  if (isValid())
    setWindowModified(true);
}

void SelectEUCountry::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);

  QIcon _iconEU = AntiquaApplIcon("european-flag");
  QIcon _iconNon = AntiquaApplIcon("dialog-warning");

  AEUCountries countries(this);

  m_edit->clear();
  m_edit->addItem(_iconNon, countries.country("XX"), "XX");

  QMapIterator<QString, QString> it(countries);
  while (it.hasNext()) {
    it.next();
    if (it.key() == "XX")
      continue;

    m_edit->addItem(_iconEU, it.value(), it.key());
  }
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectEUCountry::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() != QMetaType::QString) {
    qWarning("Invalid Data Type for SelectEUCountry.");
    return;
  }
}

void SelectEUCountry::setFocus() { m_edit->setFocus(); }

void SelectEUCountry::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectEUCountry::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() != QSqlField::Required) {
    setRequired(false);
    return;
  }
  setRequired(true);

  QString _default = field.defaultValue().toString();
  if (_default.isEmpty())
    return;

  if (m_edit->currentIndex() == 0) {
    setValue(_default);
  }
}

void SelectEUCountry::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectEUCountry::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectEUCountry::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant SelectEUCountry::getValue() {
  int _index = m_edit->currentIndex();
  return m_edit->itemData(_index, Qt::UserRole).toString();
}

const QString SelectEUCountry::popUpHints() {
  return tr("A valid Country selection is required!");
}

const QString SelectEUCountry::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
