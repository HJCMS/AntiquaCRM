// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

#include <QAbstractItemView>
#include <QDebug>

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
      int npa = obj.value("phone").toInt();
      CountryCode code;
      code.npa = QString::number(npa).rightJustified(3, '0');
      code.info = obj.value("country").toString();
      p_codes.append(code);
    }
  }
}

PhoneEdit::PhoneEdit(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setToolTip("DIN 5008/E.123");
  m_layout->addWidget(m_edit);

  QRegExp simple("^(\\d+[\\s?\\d]+)$");
  m_validator = new QRegExpValidator(simple, m_edit);
  m_edit->setValidator(m_validator);

  m_completer = new QCompleter(m_edit);
  m_completer->setMaxVisibleItems(6);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCompletionRole(Qt::EditRole);
  m_completer->setFilterMode(Qt::MatchStartsWith);
  QAbstractItemView *m_view = m_completer->popup();
  m_view->setAlternatingRowColors(true);
  m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_view->setSelectionMode(QAbstractItemView::SingleSelection);

  setRequired(false);
  setModified(false);

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(dataChanged(const QString &)));
}

const QRegExp PhoneEdit::phonePattern() {
  QRegExp reg;
  reg.setPattern("^([\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return reg;
}

bool PhoneEdit::validate(const QString &phone) const {
  QRegularExpression r(phonePattern().pattern());
  QRegularExpressionMatch m = r.match(phone);
  if (phone.length() > 4 && m.hasMatch()) {
    m_edit->setStyleSheet(QString());
    return true;
  }
  m_edit->setStyleSheet("QLineEdit {selection-background-color: red;}");
  return false;
}

void PhoneEdit::dataChanged(const QString &phone) {
  setModified(true);
  validate(phone);
}

void PhoneEdit::reset() {
  m_edit->clear();
  setModified(false);
}

void PhoneEdit::setValue(const QVariant &val) {
  QString tel = val.toString().trimmed();
  if (tel.length() > 10) {
    if (tel.startsWith("0"))
      tel.insert(3, " ");
    else
      tel.insert(4, " ");

    if (objectName().contains("c_mobil_")) {
      tel.insert(7, " ");
    } else {
      tel.insert(8, " ");
    }
  }
  m_edit->setText(tel);
}

void PhoneEdit::setFocus() { m_edit->setFocus(); }

void PhoneEdit::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  if (field.type() == QVariant::String && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    QString txt(tr("Max allowed length") + " ");
    txt.append(QString::number(field.length()));
    if (m_edit->placeholderText().isEmpty())
      m_edit->setPlaceholderText(txt);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    // Nur bei Primär nummern aktivieren
    if (objectName().contains("_0"))
      setRequired(true);

    m_edit->setClearButtonEnabled(false);
  }
}

void PhoneEdit::loadDataset() {
  PhoneCountryCodeModel *model = new PhoneCountryCodeModel(m_completer);
  if (model != nullptr) {
    m_completer->setModel(model);
    model->initModel();
    m_edit->setCompleter(m_completer);
  }
}

const QVariant PhoneEdit::value() {
  QString phone = m_edit->text();
  if (isValid())
    return phone.replace(QRegExp("\\D+"), "");
  else
    return QString();
}

bool PhoneEdit::isValid() {
  QString phone = m_edit->text().trimmed();
  // Erforderlich und Leer
  if (isRequired() && phone.isEmpty())
    return false;

  // Nicht Erforderlich und Leer
  if (phone.isEmpty())
    return true;

  // Nicht leer, dann test mit Regulären ausdruck!
  return validate(phone);
}

void PhoneEdit::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
  if (objectName().contains("c_mobil_")) {
    QString info = QString("%1 (DIN 5008/E.123)").arg("049 0152 12345678");
    m_edit->setToolTip(info);
    m_edit->setPlaceholderText(info);
  } else {
    QString info = QString("%1 (DIN 5008/E.123)").arg("049 06343 1234567");
    m_edit->setToolTip(info);
    m_edit->setPlaceholderText(info);
  }
}

const QString PhoneEdit::info() { return m_edit->toolTip(); }

const QString PhoneEdit::notes() {
  QString txt = m_edit->toolTip();
  return tr("a phone input for '%1' is required.").arg(txt);
}
