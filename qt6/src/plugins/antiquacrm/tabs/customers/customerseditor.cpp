// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerseditor.h"
#include "customersconfig.h"

#include <AntiquaCRM>
#include <QDate>
#include <QLayout>

CustomersEditor::CustomersEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{CUSTOMERS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit Book"));
  setObjectName("tab_customers_editor");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("customersedit_main_layout");
  layout->setSizeConstraint(QLayout::SetMaximumSize);

  c_id = new AntiquaCRM::SerialId(this);
  layout->addWidget(c_id);

  setLayout(layout);
}

CustomersEditor::~CustomersEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void CustomersEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "c_since";
  ignoreFields << "c_changed";

  // Settings input defaults
  const QJsonObject _jobj = loadSqlConfig(CUSTOMERS_CONFIG_POINTER);

  m_tableData = new AntiquaCRM::ASqlDataQuery("customers");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }
}

bool CustomersEditor::setDataField(const QSqlField &field,
                                   const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  // qDebug() << "setDataField:" << field.name() << value;
  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget *inp =
      findChild<AntiquaCRM::AInputWidget *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setRestrictions(field);
    // Muss nach setRestrictions kommen!
    inp->setValue(value);
    return true;
  }

  if (ignoreFields.contains(key))
    return true;

  if (required) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown:" << key << "|" << value;
#else
    qWarning("Unknown Key (%s) found.", qPrintable(key));
#endif
  }
  return false;
}

void CustomersEditor::importSqlResult() {
  if (m_tableData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_tableData->getDataset());
  blockSignals(true);
  while (it.hasNext()) {
    it.next();
    QSqlField field = m_tableData->getProperties(it.key());
    setDataField(field, it.value());
  }
  blockSignals(false);

  // Suche Bilddaten
  // qint64 id = c_id->getValue().toLongLong();

  setResetModified(inputFields);
}

bool CustomersEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << query;
  return true;

  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("c_id")) {
      QSqlField field = m_tableData->getProperties("c_id");
      setDataField(field, q.value("c_id"));
    }
  }

  openSuccessMessage(tr("Bookdata saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CustomersEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<AntiquaCRM::AInputWidget *> list =
      findChildren<AntiquaCRM::AInputWidget *>(fieldPattern,
                                               Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AInputWidget *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    AntiquaCRM::AInputWidget *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->getValue();

    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->popUpHints());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(objName, cur->getValue());
  }
  list.clear();

  return data;
}

void CustomersEditor::createSqlUpdate() {
  int articleId = c_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  c_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "c_id")
      continue;

    const QString _key = it.key();
    // Nur geänderte Felder in das Update aufnehmen!
    if (!isModifiedCompare(_key, m_tableData->getValue(_key)))
      continue;

    if (m_tableData->getType(_key).id() == QMetaType::QString) {
      set.append(_key + "='" + it.value().toString() + "'");
      changes++;
    } else {
      set.append(_key + "=" + it.value().toString());
      changes++;
    }
  }

  if (changes == 0) {
    pushStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  QString sql("UPDATE customers SET ");
  sql.append(set.join(","));
  sql.append(",c_changed=CURRENT_TIMESTAMP WHERE c_id=");
  sql.append(c_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Customers success!");
    setWindowModified(false);
  }
}

void CustomersEditor::createSqlInsert() {
  int articleId = c_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  c_id->setRequired(false);

  // Die Initialisierung erfolgt in setInputFields!
  // Bei einem INSERT wird diese hier befüllt!
  if (m_tableData == nullptr || !m_tableData->isValid()) {
    qWarning("Invalid AntiquaCRM::ASqlDataQuery detected!");
    return;
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList column; // SQL Columns
  QStringList values; // SQL Values
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().isNull())
      continue;

    QString field = it.key();
    // druck einfügen
    m_tableData->setValue(field, it.value());

    column.append(field);
    if (m_tableData->getType(field).id() == QMetaType::QString) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO customers (");
  sql.append(column.join(","));
  sql.append(",c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING c_id;");
  if (sendSqlQuery(sql) && c_id->getValue().toInt() >= 1) {
    c_id->setRequired(true);
    setWindowModified(false);
  }
}

void CustomersEditor::setSaveData() {
  if (c_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  createSqlUpdate();
}

void CustomersEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void CustomersEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  emit sendLeaveEditor(); /**< Back to MainView */
}

void CustomersEditor::setRestore() { importSqlResult(); }

bool CustomersEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString c_id = QString::number(articleId);
  if (c_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_tableData->tableName();
  QString query("SELECT * FROM " + table + " WHERE c_id=" + c_id + ";");
  QSqlQuery q = m_sql->query(query);
  if (q.size() != 0) {
    QSqlRecord r = m_tableData->record();
    while (q.next()) {
      foreach (QString key, inputFields) {
        m_tableData->setValue(key, q.value(r.indexOf(key)));
      }
    }
    setLoadThumbnail(articleId);
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
    registerInputChanged();
  }

  return status;
}

bool CustomersEditor::createNewEntry() {
  setInputFields();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "c_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  registerInputChanged();
  return isEnabled();
}
