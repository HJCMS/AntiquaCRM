// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

#include <AntiquaCRM>
#include <QAbstractItemView>
#include <QFontMetrics>
#include <QRegularExpression>
#include <QSize>

namespace AntiquaCRM {

PhoneCountryCodeModel::PhoneCountryCodeModel(QObject *parent)
    : QAbstractListModel{parent} {}

int PhoneCountryCodeModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_codes.size();
}

int PhoneCountryCodeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 2;
}

QVariant PhoneCountryCodeModel::data(const QModelIndex &index, int role) const {
  if ((role & ~(Qt::DisplayRole | Qt::EditRole | Qt::ToolTipRole)) ||
      !index.isValid())
    return QVariant();

  if (role == Qt::EditRole) {
    if (index.column() == 1)
      return p_codes[index.row()].npa;
    else if (index.column() == 2)
      return p_codes[index.row()].info;
    else
      return p_codes[index.row()].npa;
  }

  if (role == Qt::DisplayRole) {
    QString ret(p_codes[index.row()].npa);
    ret.append(" ");
    ret.append(p_codes[index.row()].info);
    return ret;
  }

  if (role == Qt::ToolTipRole) {
    QString ret(p_codes[index.row()].npa);
    ret.append(" (");
    ret.append(p_codes[index.row()].info);
    ret.append(")");
    return ret;
  }

  return QVariant();
}

QVariant PhoneCountryCodeModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  Q_UNUSED(role);
  return QVariant();
}

void PhoneCountryCodeModel::initModel() {
  p_codes.clear();
  AntiquaCRM::ASharedDataFiles _db(AntiquaCRM::ASettings::getDataDir("json"));
  if (!_db.fileExists("iso_countrycodes")) {
    qWarning("Missing Country codes document!");
    return;
  }

  const QJsonDocument _doc = _db.getJson("iso_countrycodes");
  if (_doc.isEmpty())
    return;

  const QJsonArray _array = _doc.object().value("countries").toArray();
  for (int i = 0; i < _array.size(); i++) {
    const QJsonObject _obj = _array[i].toObject();
    qint64 _npa = _obj.value("phone").toInteger(0);
    if (_npa < 1)
      continue;

    CountryCode _code;
    // a national phone area must begin with 0
    _code.npa = AntiquaCRM::AUtil::zerofill(_npa, 4);
    _code.info = _obj.value("country").toString();
    p_codes.append(_code);
  }
}

PhoneEdit::PhoneEdit(QWidget *parent, const QString &name)
    : AntiquaCRM::AInputWidget{parent} {
  setObjectName(name);

  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setToolTip("DIN 5008/E.123");
  layout->addWidget(m_edit);

  m_validator = new QRegularExpressionValidator(AUtil::phoneRegExp(), m_edit);
  m_edit->setValidator(m_validator);

  initData();

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
}

bool PhoneEdit::validate(const QString &phone) const {
  bool _b = (phone.length() > 3);
  if (_b)
    _b = AntiquaCRM::AUtil::checkPhone(phone);

  if (phone.indexOf("+", 0) != -1)
    return false;

  return _b;
}

void PhoneEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  setRestrictions(_f);
  setWindowModified(false);

  m_completer = new QCompleter(m_edit);
  m_completer->setMaxVisibleItems(6);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCompletionRole(Qt::EditRole);
  m_completer->setFilterMode(Qt::MatchStartsWith);

  QAbstractItemView *m_view = m_completer->popup();
  m_view->setAlternatingRowColors(true);
  m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_view->setSelectionMode(QAbstractItemView::SingleSelection);

  PhoneCountryCodeModel *model = new PhoneCountryCodeModel(m_completer);
  if (model != nullptr) {
    m_completer->setModel(model);
    model->initModel();
    m_edit->setCompleter(m_completer);
  }
}

void PhoneEdit::valueChanged(const QString &phone) {
  bool _b = validate(phone);
  m_edit->setValidContent(_b);
  if (isRequired())
    m_edit->setIconWarning(!_b);

  setWindowModified(true);
  emit sendInputChanged();
}

void PhoneEdit::setValue(const QVariant &value) {
  if (value.metaType().id() != getType().id())
    return;

  QString _phone = value.toString().trimmed();
  _phone.replace("+", "0");
  _phone.replace(" ", "");

  bool _b = validate(_phone);
  if (!_b)
    return;

  if (_phone.length() > 10) {
    if (_phone.startsWith("0"))
      _phone.insert(3, " ");
    else
      _phone.insert(4, " ");

    if (objectName().contains("c_mobil_")) {
      _phone.insert(7, " ");
    } else {
      _phone.insert(8, " ");
    }
  }

  m_edit->setText(_phone);
  setWindowModified(false);
}

void PhoneEdit::setFocus() { m_edit->setFocus(); }

void PhoneEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void PhoneEdit::setRestrictions(const QSqlField &field) {
  int _length = field.length();
  QMetaType _type = field.metaType();
  if (_type.id() == QMetaType::QString && _length > 0) {
    m_edit->setMaxLength(_length);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  } else {
    setRequired(false);
    m_edit->setClearButtonEnabled(true);
  }

  m_edit->setPlaceholderText("DIN 5008/E.123");
}

void PhoneEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void PhoneEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
  layout->setStretch(1, 1);
}

bool PhoneEdit::isValid() { return validate(m_edit->text()); }

const QMetaType PhoneEdit::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant PhoneEdit::getValue() { return m_edit->text().trimmed(); }

const QString PhoneEdit::popUpHints() {
  return tr("Please enter a valid phone number.");
}

const QString PhoneEdit::statusHints() {
  return tr("Missing phone number ...");
}

} // namespace AntiquaCRM
