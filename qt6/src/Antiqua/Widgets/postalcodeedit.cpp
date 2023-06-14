// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodeedit.h"
#include "postalcodemodel.h"

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
           << "State:" << code.state << Qt::endl
           << "Country:" << code.country;
}
#endif

// BEGIN::PostalCodeEdit
PostalCodeEdit::PostalCodeEdit(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
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
}

const AntiquaCRM::PostalCode PostalCodeEdit::dummyCode() {
  AntiquaCRM::PostalCode _t;
  _t.plz = QString();
  _t.location = QString();
  _t.country = QString();
  _t.state = QString();
  return _t;
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
  emit sendInputChanged();
}

void PostalCodeEdit::setPostalCodeLeave() {
  QString t_plz = m_postalcode->text().trimmed();
  if (t_plz.isEmpty() || t_plz.length() < 4)
    return;

  if (m_countries->currentIndex() == 0)
    return;

  AntiquaCRM::PostalCode _pcode = getPostalCode(t_plz);

#ifdef ANTIQUA_DEVELOPEMENT
  __postalcode_debug(_pcode);
#endif

  emit sendOnLeavePostalEdit(_pcode);
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
    qWarning("Invalid PostalCodeEdit::setValue usage!");
    break;
  };
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
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(10);
  setRestrictions(_f);

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

const AntiquaCRM::PostalCode PostalCodeEdit::getPostalCode(const QString &plz) {
  if (plz.isEmpty()) {
    qWarning("Invalid usage for PostalCodeEdit::getPostalCode!");
    return dummyCode();
  }

  QString _country = getCountry();
  if (_country.isEmpty()) {
    qWarning("PostalCodeEdit::getPostalCode - no country selected!");
    return dummyCode();
  }

  PostalCodeModel *m = qobject_cast<PostalCodeModel *>(m_completer->model());
  if (m == nullptr)
    return dummyCode();

  Qt::ItemDataRole qrole = Qt::EditRole;
  for (int r = 0; r < m->rowCount(); r++) {
    QModelIndex mIndex = m->sibling(r, 0, QModelIndex());
    QVariant var = m->data(mIndex, qrole);
    if (!var.isValid())
      continue;

    if (comparePostalcode(var.toString(), plz)) {
      AntiquaCRM::PostalCode _pcode;
      _pcode.plz = var.toString();
      QVariant v_lo = m->data(m->sibling(r, 1, mIndex), qrole);
      _pcode.location = v_lo.toString();
      QVariant v_st = m->data(m->sibling(r, 2, mIndex), qrole);
      _pcode.state = v_st.toString();
      _pcode.country = _country;
      if (_pcode.state.length() > 1)
        _pcode.state.prepend(_country + "/");
      else
        _pcode.state = _country;

      return _pcode;
    }
  }
  return dummyCode();
}

QCompleter *PostalCodeEdit::getCompleter(QLineEdit *editor) {
  QStringList _buffer;
  if (getCountry().isEmpty())
    return nullptr; // nothing todo

  QString _plz = m_postalcode->text().trimmed();
  if (_plz.isEmpty() || _plz.length() < 4)
    return nullptr; // nothing todo

  PostalCodeModel *_m = qobject_cast<PostalCodeModel *>(m_completer->model());
  if (_m == nullptr || _m->rowCount() < 1)
    return nullptr; // nothing todo

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

#ifdef ANTIQUA_DEVELOPEMENT
  if (_buffer.size() < 1)
    qDebug() << "PostalCodeEdit::Completer::Size" << _buffer.size();
#endif

  QCompleter *m_cpl = new QCompleter(_buffer, editor);
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

const QMetaType PostalCodeEdit::getType() const {
  return QMetaType(QMetaType::QString);
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

} // namespace AntiquaCRM
