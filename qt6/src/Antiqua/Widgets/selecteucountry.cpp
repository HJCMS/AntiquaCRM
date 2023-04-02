// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecteucountry.h"

#include <AntiquaCRM>
#include <QPalette>

namespace AntiquaCRM {

// BEGIN::SelectEUCountryModel
SelectEUCountryModel::SelectEUCountryModel(QWidget *parent)
    : QAbstractListModel{parent}, p_palette{parent->palette()},
      euIcon(AntiquaApplIcon("european-flag")),
      nonIcon(AntiquaApplIcon("dialog-warning")) {
  p_list.clear();
}

int SelectEUCountryModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_list.size();
}

QVariant SelectEUCountryModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  int row = index.row();
  if (row > rowCount())
    return QVariant();

  const CountryRow _country = p_list[index.row()];
  switch (role) {
  case (Qt::DisplayRole):
    return _country.name;

  case (Qt::EditRole):
  case (Qt::UserRole):
    return _country.bcp47;

  case (Qt::ToolTipRole):
    return _country.name;

  case (Qt::DecorationRole):
    return (_country.bcp47 == "XX") ? nonIcon : euIcon;

  case (Qt::BackgroundRole):
    return ((row % 2) & 1) ? p_palette.alternateBase() : p_palette.base();

  case (Qt::InitialSortOrderRole):
    return _country.index;

  default:
    return QVariant();
  };

  return QVariant();
}

bool SelectEUCountryModel::initModel() {
  AntiquaCRM::AEUCountries countries(this);
  if (countries.size() < 1)
    return false;

  int row = 0;
  beginInsertRows(createIndex(row, 0), 0, countries.size());
  // No Selection (Fix sort order)
  CountryRow _item(row++, "XX", countries.country("XX"));
  p_list.append(_item);

  QMapIterator<QString, QString> it(countries);
  while (it.hasNext()) {
    it.next();
    if (it.key() != "XX")
      p_list.append(CountryRow(row++, it.key(), it.value()));
  }
  endInsertRows();

  return (p_list.size() > 0);
}

// END::SelectEUCountryModel

// BEGIN::SelectEUCountry
SelectEUCountry::SelectEUCountry(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);

  m_model = new SelectEUCountryModel(m_edit);

  initData();

  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

SelectEUCountry::~SelectEUCountry() {
  if (m_model != nullptr)
    m_model->deleteLater();
}

void SelectEUCountry::valueChanged(int) {
  if (isValid())
    setWindowModified(true);
}

void SelectEUCountry::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);

  if (m_model->initModel())
    m_edit->setModel(m_model);

  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectEUCountry::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() != QMetaType::QString) {
    qWarning("Invalid Data Type for SelectEUCountry.");
    return;
  }

  int _index = 0;
  QString _value = value.toString().trimmed();
  if (_value.length() > 2)
    _index = m_edit->findData(_value, Qt::DisplayRole, Qt::MatchStartsWith);
  else
    _index = m_edit->findData(_value, Qt::UserRole, Qt::MatchFixedString);

  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectEUCountry::setFocus() { m_edit->setFocus(); }

void SelectEUCountry::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectEUCountry::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
  }
  setRequired(false);

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
// END::SelectEUCountry

} // namespace AntiquaCRM
