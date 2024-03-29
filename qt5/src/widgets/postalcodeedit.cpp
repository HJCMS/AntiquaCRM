// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodeedit.h"

#include <AntiquaCRM>
#include <QAbstractItemView>
#include <QDebug>
#include <QJsonObject>

PostalCodeModel::PostalCodeModel(QObject *parent)
    : QAbstractListModel{parent} {}

int PostalCodeModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return p_codes.size();
}

int PostalCodeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant PostalCodeModel::data(const QModelIndex &index, int role) const {
  if ((role & ~(Qt::DisplayRole | Qt::EditRole | Qt::ToolTipRole)) ||
      !index.isValid())
    return QVariant();

  if (role == Qt::EditRole) {
    if (index.column() == 1)
      return p_codes[index.row()].location;
    else if (index.column() == 2)
      return p_codes[index.row()].state;
    else
      return p_codes[index.row()].plz;
  }

  if (role == Qt::DisplayRole) {
    QString ret(p_codes[index.row()].plz);
    ret.append(" ");
    ret.append(p_codes[index.row()].location);
    return ret;
  }

  if (role == Qt::ToolTipRole) {
    QString ret(p_codes[index.row()].plz);
    ret.append(" (");
    ret.append(p_codes[index.row()].location);
    QString st = p_codes[index.row()].state;
    if (!st.isEmpty()) {
      ret.append("," + st);
    }
    ret.append(")");
    return ret;
  }

  return QVariant();
}

QVariant PostalCodeModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  Q_UNUSED(section);
  Q_UNUSED(orientation);
  Q_UNUSED(role);
  return QVariant();
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

PostalCodeEdit::PostalCodeEdit(QWidget *parent) : InputEdit{parent} {

  m_countries = new AntiquaComboBox(this);
  m_countries->setToolTip(tr("Supported countries"));
  m_layout->addWidget(m_countries);

  m_postalcode = new QLineEdit(this);
  m_postalcode->setMaxLength(12);
  m_postalcode->setMaximumWidth(100);
  m_postalcode->setPlaceholderText(tr("Postalcode"));
  m_postalcode->setClearButtonEnabled(false);
  m_layout->addWidget(m_postalcode);

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
  setRequired(false);
  setModified(false);

  connect(m_countries, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));

  connect(m_postalcode, SIGNAL(editingFinished()), this,
          SLOT(postalReadyToLeave()));
}

bool PostalCodeEdit::comparePostalcode(const QString &source,
                                       const QString &input) const {
  QString src(source);
  if ((input.length() > src.length()) && input.startsWith("0"))
    src.prepend("0");

  return (src == input);
}

void PostalCodeEdit::loadDataset() {
  m_countries->clear();
  m_countries->insertItem(0, tr("Without disclosures"), QString());

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

const QStringList PostalCodeEdit::locations(const QString &fromCode) {
  QStringList locations;
  QString t_plz = fromCode.trimmed();
  if (t_plz.isEmpty() || t_plz.length() < 4)
    return locations;

  PostalCodeModel *m = qobject_cast<PostalCodeModel *>(m_completer->model());
  if (m != nullptr) {
    Qt::ItemDataRole qrole = Qt::EditRole;
    for (int r = 0; r < m->rowCount(); r++) {
      QModelIndex mIndex = m->sibling(r, 0, QModelIndex());
      QVariant var = m->data(mIndex, qrole);
      if (!var.isValid())
        continue;

      if (comparePostalcode(var.toString(), t_plz)) {
        QVariant v_lo = m->data(m->sibling(r, 1, mIndex), qrole);
        locations << v_lo.toString();
      }
    }
  }
  return locations;
}

void PostalCodeEdit::dataChanged(int index) {
  QString table = m_countries->itemData(index, Qt::UserRole).toString();
  if (table.isEmpty())
    return;

  PostalCodeModel *model = new PostalCodeModel(m_completer);
  if (model != nullptr) {
    m_completer->setModel(model);
    model->initModel(table);
    m_postalcode->setCompleter(m_completer);
  }
}

void PostalCodeEdit::postalReadyToLeave() {
  QString t_plz = m_postalcode->text().trimmed();
  if (t_plz.isEmpty() || t_plz.length() < 4)
    return;

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
        emit sendOnLeavePostalEdit(code);
        break;
      }
    }
  }
}

void PostalCodeEdit::reset() {
  m_postalcode->clear();
  m_countries->setCurrentIndex(0);
  setModified(false);
}

void PostalCodeEdit::setValue(const QVariant &val) {
  if (val.type() == QVariant::String) {
    m_postalcode->setText(val.toString());
    return;
  }
  QString str = QString::number(val.toInt());
  m_postalcode->setText(str);
}

void PostalCodeEdit::setFocus() { m_postalcode->setFocus(); }

void PostalCodeEdit::findCountry(const QString &name) {
  QString search(name.trimmed());
  if (search.isEmpty())
    return;

  if (name.contains("/")) {
    QStringList l = name.split("/");
    search = l.first().trimmed();
  }

  int index = m_countries->findData(search, Qt::DisplayRole);
  if (index > 0)
    m_countries->setCurrentIndex(index);
}

const QVariant PostalCodeEdit::value() {
  QString buffer = m_postalcode->text();
  return buffer.trimmed();
}

bool PostalCodeEdit::isValid() {
  if (isRequired() && m_postalcode->text().isEmpty())
    return false;

  return true;
}

void PostalCodeEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void PostalCodeEdit::setInfo(const QString &info) {
  m_postalcode->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString PostalCodeEdit::country() {
  if (m_countries->currentIndex() > 0)
    return m_countries->currentText();

  return QString();
}

const QString PostalCodeEdit::info() { return toolTip(); }

const QString PostalCodeEdit::notes() {
  return tr("a valid Postalcode is required!");
}
