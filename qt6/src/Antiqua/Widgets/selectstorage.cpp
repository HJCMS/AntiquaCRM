// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectstorage.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace AntiquaCRM {

SelectStorage::SelectStorage(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_select = new AntiquaCRM::AComboBox(this);
  m_select->setToolTip(tr("Storage Location"));
  m_select->setWithoutDisclosures(QString());
  m_select->setMaxVisibleItems(20);
  m_select->setWhatsThis(tr("Changes the Storage location of this Article."));
  layout->addWidget(m_select);

  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("Storage Location"));
  m_edit->setMinimumWidth(130);
  layout->addWidget(m_edit);

  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);

  connect(m_select, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(filterChanged(const QString &)));
}

void SelectStorage::valueChanged(int index) {
  if (index > 0)
    setWindowModified(true);
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
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
    _index = m_select->findData(value, Qt::UserRole);
    break;

  case (QMetaType::QString): {
    QString find = value.toString().trimmed();
    const QRegularExpression pattern("\\b" + find.toLower() + "\\b",
                                     QRegularExpression::CaseInsensitiveOption);
    _index = m_select->findData(pattern, Qt::DisplayRole, // Display
                                Qt::MatchRegularExpression);
  } break;

  default:
    qWarning("Invalid given Data Type in SelectStorage.");
#ifdef ANTIQUA_DEVELOPEMENT
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
    QJsonDocument jdoc = dataFile.getJson(key);
    QJsonArray arr = jdoc.object().value(key).toArray();
    if (arr.size() > 0) {
      for (int i = 0; i < arr.size(); i++) {
        QJsonObject jo = arr[i].toObject();
        int index = jo.value("id").toInt();
        if (index == 0)
          continue;

        QString display = jo.value("storage").toString();
        display.append(" - ");
        display.append(jo.value("identifier").toString());
        m_select->insertItem(index, display, index);
      }
    }
  } else {
    qWarning("Storage:No storagelocations.json - fallback to SQL query!");
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QString sql("SELECT * FROM ref_storage_location ORDER BY sl_id;");
    QSqlQuery q = m_sql->query(sql);
    if (q.size() > 0) {
      while (q.next()) {
        int index = q.value("sl_id").toInt();
        if (index == 0)
          continue;

        QString display = q.value("sl_storage").toString();
        display.append(" - ");
        display.append(q.value("sl_identifier").toString());
        m_select->insertItem(index, display, index);
      }
    }
  }
}

void SelectStorage::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
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

const QVariant SelectStorage::getValue() {
  return m_select->itemData(m_select->currentIndex(), Qt::UserRole);
}

const QString SelectStorage::popUpHints() {
  return tr("Storage location is required and must set.");
}

const QString SelectStorage::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
