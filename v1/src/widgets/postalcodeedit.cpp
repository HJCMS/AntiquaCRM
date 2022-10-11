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
  m_countries->insertItem(0, tr("Without disclosures"), QString());
  m_layout->addWidget(m_countries);

  m_postalcode = new QLineEdit(this);
  m_postalcode->setMaxLength(12);
  m_postalcode->setMaximumWidth(100);
  m_postalcode->setPlaceholderText(tr("Postalcode"));
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
  loadDataset();
  setRequired(true);

  connect(m_countries, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));

  connect(m_postalcode, SIGNAL(editingFinished()), this,
          SLOT(postalReadyToLeave()));
}

void PostalCodeEdit::loadDataset() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString field("p_country,p_table");
  QSqlQuery q = m_sql->query("SELECT " + field + " FROM ui_postalcodes;");
  if (q.size() > 0) {
    while (q.next()) {
      m_countries->addItem(q.value("p_country").toString(),
                           q.value("p_table").toString());
    }
  }
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
  // setModified(true);
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

      if (var.toString() == t_plz) {
        AntiquaCRM::PostalCode code;
        code.plz = var.toString();
        QVariant v_lo = m->data(m->sibling(r, 1, mIndex), qrole);
        code.location = v_lo.toString();
        QVariant v_st = m->data(m->sibling(r, 2, mIndex), qrole);
        code.state = v_st.toString();
        // qDebug() << Q_FUNC_INFO << t_plz << v_lo << v_st;
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

const QVariant PostalCodeEdit::value() {
  QString buffer = m_postalcode->text();
  return buffer.trimmed();
}

bool PostalCodeEdit::isValid() {
  if (isRequired() && m_postalcode->text().isEmpty())
    return false;

  return true;
}

void PostalCodeEdit::setInfo(const QString &info) {
  m_postalcode->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString PostalCodeEdit::info() { return toolTip(); }

const QString PostalCodeEdit::notes() {
  return tr("a valid Postalcode is required!");
}
