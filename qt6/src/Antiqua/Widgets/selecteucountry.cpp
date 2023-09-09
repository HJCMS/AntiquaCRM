// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecteucountry.h"
#include "antiquaicon.h"

#include <AntiquaCRM>
#include <QPalette>

namespace AntiquaCRM {

// BEGIN::CountryItem
EUCountry::EUCountry(int ro, QString is, QString na) {
  index = ro;
  iso = is;
  name = na;
}
// END::CountryItem

// BEGIN::SelectEUCountryModel
SelectEUCountryModel::SelectEUCountryModel(QWidget *parent)
    : QAbstractListModel{parent}, p_palette{parent->palette()},
      euIcon(AntiquaCRM::antiquaIcon("european-flag")),
      dwIcon(AntiquaCRM::antiquaIcon("dialog-warning")) {
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

  const EUCountry _country = p_list[index.row()];
  const QString _bcp47(_country.iso);
  switch (role) {
  case (Qt::DisplayRole):
    return _country.name;

  case (Qt::EditRole):
  case (Qt::UserRole):
    return _bcp47;

  case (Qt::ToolTipRole):
    return _country.name;

  case (Qt::DecorationRole):
    return (_bcp47 == "XX" || _bcp47.isEmpty()) ? dwIcon : euIcon;

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
  AntiquaCRM::ASharedDataFiles files(AntiquaCRM::ASettings::getDataDir("json"));
  QJsonObject obj = files.getJson("european_countries").object();
  QStringList _keys = obj.keys();
  if (_keys.size() > 0) {
    int row = 0;
    QJsonArray _arr = obj.value("european_countries").toArray();
    beginInsertRows(createIndex(row, 0), 0, _arr.size() + 1);
    // No Selection (Fix sort order)
    p_list.append(EUCountry(row++, QString(), tr("Without disclosures")));
    p_list.append(EUCountry(row++, "XX", tr("Non European Country")));
    for (int i = 0; i < _arr.size(); i++) {
      QJsonObject item = _arr[i].toObject();
      EUCountry eu(row++, item.value("code").toString(),
                   item.value("country").toString());
      p_list.append(eu);
    }
    endInsertRows();
  }
  return (p_list.size() > 0);
}

int SelectEUCountryModel::size() { return p_list.size(); }

// END::SelectEUCountryModel

// BEGIN::SelectEUCountry
SelectEUCountry::SelectEUCountry(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
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

int SelectEUCountry::noMemberIndex() {
  return m_edit->findData("XX", Qt::UserRole, Qt::MatchFixedString);
}

void SelectEUCountry::valueChanged(int index) {
  if (index == 0)
    return;

  if (m_model->size() < 1)
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void SelectEUCountry::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setLength(2);
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
  if (_value.length() == 2) // bcp47
    _index = m_edit->findData(_value, Qt::UserRole, Qt::MatchFixedString);
  else
    _index = m_edit->findData(_value, Qt::DisplayRole, Qt::MatchStartsWith);

  if (_index > 0) {
    m_edit->setCurrentIndex(_index);
    return;
  }

  qInfo("Changes for '%s' to non EU Country.", qPrintable(_value));
  m_edit->setCurrentIndex(noMemberIndex());
}

void SelectEUCountry::setFocus() { m_edit->setFocus(); }

void SelectEUCountry::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectEUCountry::setRestrictions(const QSqlField &field) {
  if (m_edit->currentIndex() == 0) {
    QString _default = field.defaultValue().toString();
    if (_default.isEmpty())
      return;

    setValue(_default);
  }
  setRequired((field.requiredStatus() == QSqlField::Required));
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

const QMetaType SelectEUCountry::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant SelectEUCountry::getValue() {
  int _index = m_edit->currentIndex();
  if (_index == 0)
    _index = noMemberIndex();

  QString _v = m_edit->itemData(_index, Qt::UserRole).toString();
  return (_v.length() == 2) ? _v.toUpper() : QString();
}

const QString SelectEUCountry::popUpHints() {
  return tr("A valid Country selection is required!");
}

const QString SelectEUCountry::statusHints() { return popUpHints(); }
// END::SelectEUCountry

} // namespace AntiquaCRM
