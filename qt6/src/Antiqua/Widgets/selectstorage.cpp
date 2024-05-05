// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectstorage.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace AntiquaCRM {

SelectStorage::SelectStorage(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_select = new AntiquaCRM::AComboBox(this);
  m_select->setToolTip(tr("Storage Location"));
  m_select->setWithoutDisclosures();
  m_select->setMaxVisibleItems(20);
  m_select->setWhatsThis(tr("Changes the Storage location for this Article."));
  layout->addWidget(m_select);

  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("Search location"));
  m_edit->setMinimumWidth(130);
  layout->addWidget(m_edit);

  layout->addStretch(0);

  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);

  connect(m_select, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(filterChanged(const QString &)));
}

void SelectStorage::valueChanged(int index) {
  if (index < 1)
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void SelectStorage::filterChanged(const QString &filter) {
  int index = 0;
  if (filter.length() < 3) {
    index = m_select->findText(filter, Qt::MatchStartsWith);
  } else {
    index = m_select->findText(filter, Qt::MatchContains);
  }

  if (index != 0)
    m_select->setCurrentIndex(index);
}

void SelectStorage::setValue(const QVariant &value) {
  int _index = -1;
  switch (value.metaType().id()) {
  case (QMetaType::Int):
  case (QMetaType::LongLong):
  case (QMetaType::Double):
    _index = m_select->findData(value, Qt::UserRole, Qt::MatchExactly);
    break;

  case (QMetaType::QString): {
    QString _find = value.toString().trimmed();
    const QRegularExpression pattern("\\b" + _find.toLower() + "\\b",
                                     QRegularExpression::CaseInsensitiveOption);
    _index = m_select->findData(pattern, Qt::DisplayRole, // Display
                                Qt::MatchRegularExpression);
  } break;

  default:
    qWarning("Invalid given Data Type in SelectStorage.");
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << "SelectStorage Requires type int but get:" << value;
#endif
    break;
  };

  if (_index < 1)
    return;

  m_select->setCurrentIndex(_index);
}

void SelectStorage::setFocus() { m_select->setFocus(); }

void SelectStorage::reset() {
  m_edit->clear();
  m_select->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectStorage::initData() {
  QString key("storagelocations");
  AntiquaCRM::ASharedDataFiles dataFile;
  if (dataFile.fileExists(key)) {
    QJsonDocument _jd = dataFile.getJson(key);
    QJsonArray _arr = _jd.object().value(key).toArray();
    if (_arr.size() > 0) {
      for (int i = 0; i < _arr.size(); i++) {
        QJsonObject _jo = _arr[i].toObject();
        int _id = _jo.value("id").toInt();
        if (_id == 0)
          continue;

        QString _str = _jo.value("storage").toString();
        _str.append(" - ");
        _str.append(_jo.value("identifier").toString());
        m_select->insertItem(_id, _str, _id);
      }
    }
  } else {
    qWarning("Storage:No storagelocations.json - fallback to SQL query!");
    AntiquaCRM::ASqlCore aSql(this);
    QString _sql("SELECT * FROM ref_storage_location ORDER BY sl_id;");
    QSqlQuery _q = aSql.query(_sql);
    if (_q.size() > 0) {
      while (_q.next()) {
        int index = _q.value("sl_id").toInt();
        if (index == 0)
          continue;

        QString _str = _q.value("sl_storage").toString();
        _str.append(" - ");
        _str.append(_q.value("sl_identifier").toString());
        m_select->insertItem(index, _str, index);
      }
      _q.clear();
    }
#ifdef ANTIQUA_DEVELOPMENT
    else if (!aSql.lastError().isEmpty()) {
      qDebug() << Q_FUNC_INFO << _sql << aSql.lastError();
    }
#endif
  }
}

const QStringList SelectStorage::getCompartments() {
  QStringList _list;
  int _id = getValue().toInt();
  if (_id < 1)
    return _list;

  QString _sql("SELECT sc_storage_compartments");
  _sql.append(" FROM ref_storage_compartment");
  _sql.append(" WHERE sc_storage_location=" + QString::number(_id));
  _sql.append(" ORDER BY sc_storage_location");
  _sql.append(" LIMIT 1;");

  AntiquaCRM::ASqlCore aSql(this);
  QSqlQuery _q = aSql.query(_sql);
  if (_q.size() > 0) {
    _q.next();
    _list = _q.value("sc_storage_compartment").toStringList();
    _q.clear();
  }
#ifdef ANTIQUA_DEVELOPMENT
  else if (!aSql.lastError().isEmpty()) {
    qDebug() << Q_FUNC_INFO << _sql << aSql.lastError();
  }
#endif
  return _list;
}

void SelectStorage::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectStorage::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectStorage::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectStorage::isValid() {
  if (isRequired() && m_select->currentIndex() < 1)
    return false;

  return true;
}

const QMetaType SelectStorage::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectStorage::getValue() {
  return m_select->itemData(m_select->currentIndex(), Qt::UserRole);
}

const QJsonObject SelectStorage::getBookcardData() {
  qint64 _id = getValue().toLongLong();
  AntiquaCRM::ASqlFiles _tpl("query_storage_by_id");
  if (!_tpl.openTemplate())
    return QJsonObject();

  _tpl.setWhereClause("sl_id=" + QString::number(_id));
  AntiquaCRM::ASqlCore aSql(this);
  QString _sql(_tpl.getQueryContent());
  QSqlQuery _q = aSql.query(_sql);
  if (_q.size() > 0) {
    _q.next();
    QJsonObject _card;
    _card.insert("storage", _q.value("storage_id").toLongLong());
    _card.insert("name", _q.value("storage_name").toString());
    _card.insert("category", _q.value("storage_category").toString());
    return _card;
  }
  return QJsonObject();
}

const QString SelectStorage::popUpHints() {
  return tr("Storage location is required and must set.");
}

const QString SelectStorage::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
