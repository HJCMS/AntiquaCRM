// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

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
  AntiquaCRM::ASharedDataFiles file(AntiquaCRM::ASettings::getDataDir("json"));
  if (file.fileExists("iso_countrycodes")) {
    QJsonDocument jdoc = file.getJson("iso_countrycodes");
    QJsonArray arr = jdoc.object().value("countries").toArray();
    for (int i = 0; i < arr.size(); i++) {
      QJsonObject obj = arr[i].toObject();
      qint64 npa = obj.value("phone").toInt();
      CountryCode code;
      code.npa = AntiquaCRM::AUtil::zerofill(npa, 3);
      code.info = obj.value("country").toString();
      p_codes.append(code);
    }
  }
}

PhoneEdit::PhoneEdit(const QString &name, QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  setObjectName(name);
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setToolTip("DIN 5008/E.123");

  QRegularExpression simple("^(0\\d+[\\s?\\d]+)$");
  m_validator = new QRegularExpressionValidator(simple, m_edit);
  m_edit->setValidator(m_validator);

  layout->addWidget(m_edit);
  initData();

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
}

PhoneEdit::PhoneEdit(QWidget *parent) : PhoneEdit{"phone_edit", parent} {}

bool PhoneEdit::validate(const QString &phone) const {
  bool check = AntiquaCRM::AUtil::checkPhone(phone);
  m_edit->isValidContent(check);
  return check;
}

void PhoneEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  if (!objectName().isEmpty())
    _f.setName(objectName());

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
  validate(phone);
  setWindowModified(true);
}

void PhoneEdit::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::QString)
    return;

  QString _phone = value.toString();
  _phone.replace("+", "0");
  _phone.replace(" ", "");

  if (!validate(_phone))
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
}

void PhoneEdit::setFocus() { m_edit->setFocus(); }

void PhoneEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void PhoneEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  }
  // Secundary phone ...
  if (!objectName().contains("phone_0")) {
    setRequired(false);
    m_edit->setClearButtonEnabled(true);
  }

  QMetaType _type = field.metaType();
  if (_type.id() == QMetaType::QString && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    QString _txt(tr("Max allowed length") + " ");
    _txt.append(QString::number(field.length()));
    _txt.append(".");
    m_edit->setPlaceholderText(_txt);
  }

  if (field.length() < 1)
    return;

  QString _phone;
  _phone.append(_phone.leftJustified(field.length(), QChar('@')));

  QFontMetrics _m(m_edit->font());
  qsizetype _w = _m.size(Qt::TextSingleLine, _phone).width();
  m_edit->setMaximumWidth(((_w < 80) ? _w : (_w / 2)));
}

void PhoneEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void PhoneEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = setTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool PhoneEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return validate(m_edit->text());
}

const QVariant PhoneEdit::getValue() { return m_edit->text(); }

const QString PhoneEdit::popUpHints() {
  return tr("Please enter a valid phone number.");
}

const QString PhoneEdit::statusHints() {
  return tr("Missing phone number ...");
}

} // namespace AntiquaCRM
