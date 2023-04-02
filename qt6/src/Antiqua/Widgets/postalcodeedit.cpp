// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodeedit.h"

#include <AntiquaCRM>
#include <QAbstractItemView>
#include <QApplication>
#include <QDebug>
#include <QJsonObject>

namespace AntiquaCRM {

#ifdef ANTIQUA_DEVELOPEMENT
void __postalcode_debug(const AntiquaCRM::PostalCode &code) {
  qDebug() << "PostalCode:" << code.plz << Qt::endl
           << "Location:" << code.location << Qt::endl
           << "State:" << code.state;
}
#endif

// BEGIN::PostalCodeModel
PostalCodeModel::PostalCodeModel(QObject *parent) : QAbstractListModel{parent} {
  p_palette = qApp->palette();
}

int PostalCodeModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_codes.size();
}

int PostalCodeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant PostalCodeModel::data(const QModelIndex &index, int role) const {
  const QVariant _empty;
  if (!index.isValid())
    return _empty;

  int _row = index.row();
  const AntiquaCRM::PostalCode _code = p_codes[_row];
  if (_code.plz.isEmpty())
    return _empty;

  switch (role) {
  case (Qt::UserRole):
  case (Qt::EditRole): {
    if (index.column() == 1)
      return _code.location;
    else if (index.column() == 2)
      return _code.state;
    else
      return _code.plz;
  } break;

  case (Qt::DisplayRole): {
    QString _v(_code.plz);
    _v.append(" ");
    _v.append(_code.location);
    return _v;
  } break;

  case (Qt::ToolTipRole): {
    QString _v(_code.plz);
    _v.append(" (");
    _v.append(_code.location);
    QString st = _code.state;
    if (!st.isEmpty()) {
      _v.append("," + st);
    }
    _v.append(")");
    return _v;
  } break;

  case (Qt::BackgroundRole):
    return ((_row % 2) & 1) ? p_palette.alternateBase() : p_palette.base();

  default:
    return _empty;
  };

  return _empty;
}

void PostalCodeModel::initModel(const QString &country) {
  if (country.isEmpty())
    return;

  beginResetModel();
  p_codes.clear();
  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(QString("postalcodes"))) {
    QJsonDocument jdoc = dataFile.getJson("postalcodes");
    QJsonArray arr = jdoc.object().value(country).toArray();
    if (arr.size() > 0) {
      for (int i = 0; i < arr.size(); i++) {
        QJsonObject jobj = arr[i].toObject();
        AntiquaCRM::PostalCode code;
        code.plz = QString::number(jobj.value("plz").toInt());
        code.location = jobj.value("location").toString();
        code.state = jobj.value("state").toString();
        p_codes.append(code);
      }
    }
  } else {
    qWarning("PostalCodeEdit:No postalcodes.json - fallback to SQL query!");
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QString fields("p_plz,p_location,p_state");
    QSqlQuery q = m_sql->query("SELECT " + fields + " FROM " + country + ";");
    if (q.size() > 0) {
      p_codes.clear();
      while (q.next()) {
        AntiquaCRM::PostalCode code;
        code.plz = QString::number(q.value("p_plz").toInt());
        code.location = q.value("p_location").toString();
        code.state = q.value("p_state").toString();
        p_codes.append(code);
      }
    }
  }
  endResetModel();
}
// END::PostalCodeModel

// BEGIN::PostalCodeEdit
PostalCodeEdit::PostalCodeEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_countries = new AComboBox(this);
  m_countries->setToolTip(tr("Supported countries"));
  layout->addWidget(m_countries);

  m_postalcode = new ALineEdit(this);
  m_postalcode->setToolTip(tr("Postal Code"));
  m_postalcode->setPlaceholderText(m_postalcode->toolTip());
  m_postalcode->setClearButtonEnabled(false);
  m_postalcode->setMaximumWidth(100);
  layout->addWidget(m_postalcode);

  m_completer = new QCompleter(m_postalcode);
  m_completer->setMaxVisibleItems(6);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCompletionRole(Qt::EditRole);
  m_completer->setFilterMode(Qt::MatchStartsWith);

  QAbstractItemView *m_view = m_completer->popup();
  m_view->setAlternatingRowColors(true);
  m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_view->setSelectionMode(QAbstractItemView::SingleSelection);
  m_view->setMinimumWidth(200);

  setTabOrder(m_countries, m_postalcode);

  connect(m_countries, SIGNAL(currentIndexChanged(int)),
          SLOT(valueChanged(int)));

  connect(m_postalcode, SIGNAL(editingFinished()), SLOT(setPostalCodeLeave()));
  connect(m_postalcode, SIGNAL(sendFocusOut()), SLOT(setPostalCodeLeave()));
}

bool PostalCodeEdit::comparePostalcode(const QString &source,
                                       const QString &input) const {
  QString src(source);
  if ((input.length() > src.length()) && input.startsWith("0"))
    src.prepend("0");

  return (src == input);
}

void PostalCodeEdit::valueChanged(int index) {
  if (index == 0) {
    m_postalcode->clear();
    emit sendResetDependencies();
    return;
  }

  QString table = m_countries->itemData(index, Qt::UserRole).toString();
  if (table.isEmpty())
    return;

  PostalCodeModel *model = new PostalCodeModel(m_completer);
  if (model != nullptr) {
    m_completer->setModel(model);
    model->initModel(table);
    m_postalcode->setCompleter(m_completer);
    emit sendResetDependencies();
  }
}

void PostalCodeEdit::setPostalCodeLeave() {
  QString t_plz = m_postalcode->text().trimmed();
  if (t_plz.isEmpty() || t_plz.length() < 4)
    return;

  if (m_countries->currentIndex() == 0)
    return;

  // Selected country
  const QString _country = m_countries->currentText();

  PostalCodeModel *m = qobject_cast<PostalCodeModel *>(m_completer->model());
  if (m != nullptr) {
    Qt::ItemDataRole qrole = Qt::EditRole;
    for (int r = 0; r < m->rowCount(); r++) {
      QModelIndex mIndex = m->sibling(r, 0, QModelIndex());
      QVariant var = m->data(mIndex, qrole);
      if (!var.isValid())
        continue;

      if (comparePostalcode(var.toString(), t_plz)) {
        AntiquaCRM::PostalCode code;
        code.plz = var.toString();
        QVariant v_lo = m->data(m->sibling(r, 1, mIndex), qrole);
        code.location = v_lo.toString();
        QVariant v_st = m->data(m->sibling(r, 2, mIndex), qrole);
        code.state = v_st.toString();
        if (code.state.length() > 1)
          code.state.prepend(_country + "/");
        else
          code.state = _country;

#ifdef ANTIQUA_DEVELOPEMENT
        __postalcode_debug(code);
#endif
        emit sendOnLeavePostalEdit(code);
        break;
      }
    }
  }
}

void PostalCodeEdit::setCountry(const QString &country) {
  QString search(country.trimmed());
  if (search.isEmpty())
    return;

  if (search.contains("/")) {
    QStringList l = search.split("/");
    search = l.first().trimmed();
  }

  int index = m_countries->findData(search, Qt::DisplayRole);
  if (index > 0) {
    m_countries->setCurrentIndex(index);
  }
}

void PostalCodeEdit::setValue(const QVariant &value) {
  switch (value.metaType().id()) {
  case (QMetaType::QString): {
    m_postalcode->setText(value.toString());
  } break;

  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong): {
    QString str = QString::number(value.toInt());
    m_postalcode->setText(str);
  } break;

  default:
    return;
  };
  // @note Nur dann, wenn auch das Land gesetzt ist!
  if (m_countries->currentIndex() > 0)
    setPostalCodeLeave();
}

void PostalCodeEdit::setFocus() {
  // focus lineedit
  m_postalcode->setFocus();
}

void PostalCodeEdit::reset() {
  m_countries->setCurrentIndex(0);
  m_postalcode->clear();
  setWindowModified(false);
}

void PostalCodeEdit::initData() {
  m_countries->clear();
  m_countries->setWithoutDisclosures(QString());

  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(QString("postalcodes"))) {
    QJsonDocument jdoc = dataFile.getJson("postalcodes");
    QJsonObject tables = jdoc.object().value("tables").toObject();
    foreach (QString t, tables.keys()) {
      m_countries->addItem(tables[t].toString(), t);
    }
    return;
  }
  // SQL
#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Using SQL Database for PostalCode!");
#endif
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString field("p_country,p_table");
  QSqlQuery q = m_sql->query("SELECT " + field + " FROM ui_postalcodes;");
  if (q.size() > 0) {
    while (q.next()) {
      m_countries->addItem(q.value("p_country").toString(),
                           q.value("p_table").toString());
    }
    qWarning("PostalCode from SQL Database - non cachefile found!");
  }
}

const QString PostalCodeEdit::getCountry() {
  if (m_countries->currentIndex() > 0)
    return m_countries->currentText();

  return QString();
}

QCompleter *PostalCodeEdit::getLocations(QWidget *parent) {
  QStringList _buffer;
  if (getCountry().isEmpty())
    return nullptr; // nothing todo

  QString _plz = m_postalcode->text().trimmed();
  if (_plz.isEmpty() || _plz.length() < 4)
    return nullptr; // nothing todo

  PostalCodeModel *_m = qobject_cast<PostalCodeModel *>(m_completer->model());
  if (_m == nullptr || _m->rowCount() < 1)
    return nullptr;

  for (int r = 0; r < _m->rowCount(); r++) {
    QModelIndex _index = _m->sibling(r, 0, QModelIndex());
    QVariant _var = _m->data(_index, Qt::EditRole);
    if (!_var.isValid())
      continue;

    if (comparePostalcode(_var.toString(), _plz)) {
      QVariant _value = _m->data(_m->sibling(r, 1, _index), Qt::EditRole);
      _buffer << _value.toString();
    }
  }

  QCompleter *m_cpl = new QCompleter(_buffer, parent);
  m_cpl->setCaseSensitivity(Qt::CaseInsensitive);
  m_cpl->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  return m_cpl;
}

void PostalCodeEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  m_postalcode->setMaxLength(field.length());
}

void PostalCodeEdit::setInputToolTip(const QString &tip) {
  m_postalcode->setToolTip(tip);
}

void PostalCodeEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_postalcode);
}

bool PostalCodeEdit::isValid() {
  if (isRequired() && m_postalcode->text().isEmpty())
    return false;

  return true;
}

const QVariant PostalCodeEdit::getValue() {
  return m_postalcode->text().trimmed();
}

const QString PostalCodeEdit::popUpHints() {
  return tr("a valid postal code is required!");
}

const QString PostalCodeEdit::statusHints() {
  return tr("a valid postal code is required!");
}
// END::PostalCodeEdit

// BEGIN::PostalCodeState
PostalCodeState::PostalCodeState(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
}

void PostalCodeState::initData() {
  // unused in this
}

void PostalCodeState::setCountry(const AntiquaCRM::PostalCode &code) {
  if (code.state.isEmpty())
    return;

  m_edit->setText(code.state);
}

void PostalCodeState::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setText(value.toString());
    return;
  }
  QString str = QString::number(value.toInt());
  m_edit->setText(str);
}

void PostalCodeState::setFocus() { m_edit->setFocus(); }

void PostalCodeState::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void PostalCodeState::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != QMetaType::QString)
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeState::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeState::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeState::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QVariant PostalCodeState::getValue() { return m_edit->text().trimmed(); }

const QString PostalCodeState::popUpHints() {
  return tr("Missing Country/State for Postalcode!");
}

const QString PostalCodeState::statusHints() {
  return tr("Missing Country/State for Postalcode!");
}
// END::PostalCodeState

// BEGIN::PostalCodeLocation
PostalCodeLocation::PostalCodeLocation(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
}

void PostalCodeLocation::initData() {}

void PostalCodeLocation::setLocation(const AntiquaCRM::PostalCode &code) {
  if (code.location.isEmpty())
    return;

  m_edit->setText(code.location);

  PostalCodeEdit *o_pce = qobject_cast<AntiquaCRM::PostalCodeEdit *>(sender());
  if (o_pce == nullptr)
    return;

  QCompleter *m_cpl = o_pce->getLocations(this);
  if (m_cpl == nullptr)
    return;

  m_edit->setClearButtonEnabled(true);
  m_edit->setCompleter(m_cpl);
  m_edit->setCompleterAction(true);
}

void PostalCodeLocation::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setText(value.toString());
    return;
  }
  QString str = QString::number(value.toInt());
  m_edit->setText(str);
}

void PostalCodeLocation::setFocus() { m_edit->setFocus(); }

void PostalCodeLocation::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void PostalCodeLocation::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != QMetaType::QString)
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeLocation::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeLocation::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeLocation::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QVariant PostalCodeLocation::getValue() {
  return m_edit->text().trimmed();
}

const QString PostalCodeLocation::popUpHints() {
  return tr("Missing Location for Postalcode!");
}

const QString PostalCodeLocation::statusHints() {
  return tr("Missing Location for Postalcode!");
}

// END::PostalCodeLocation
} // namespace AntiquaCRM
