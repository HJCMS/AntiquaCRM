// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerseditor.h"
#include "customersdata.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QLayout>
#include <QMessageBox>

CustomersEditor::CustomersEditor(QWidget *parent)
    : InventoryEditor{"^c_[a-z_]+\\b$", parent} {
  setObjectName("customers_editor");
  setWindowTitle(tr("Customer Book"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  m_dataWidget = new CustomersData(this);
  mainLayout->addWidget(m_dataWidget);

  setLayout(mainLayout);
  setEnabled(false);
}

void CustomersEditor::setInputFields() {
  m_tableData = new AntiquaCRM::ASqlDataQuery("customers");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // TODO completers
  m_dataWidget->c_postalcode->loadDataset();
}

bool CustomersEditor::setDataField(const QSqlField &field,
                                   const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);

  InputEdit *inp = findChild<InputEdit *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    inp->setProperties(field);
    return true;
  }

  qDebug() << "Unknown:" << key << "|" << value << "|" << required;
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

  // m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool CustomersEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << query;
  return true;
  /*
    QSqlQuery q = m_sql->query(query);
    if (q.lastError().type() != QSqlError::NoError) {
      qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
      return false;
    } else {
      if (q.next()) {
        if (!q.isNull("ib_id")) {
          ib_id->setValue(q.value("ib_id"));
        }
      }
      openSuccessMessage(tr("Bookdata saved successfully!"));
      setResetModified(inputFields);
      return true;
    }
  */
}

const QHash<QString, QVariant> CustomersEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, Qt::FindChildrenRecursively);
  QList<InputEdit *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    InputEdit *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->value();
    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(objName, cur->value());
  }
  list.clear();

  return data;
}

void CustomersEditor::createSqlUpdate() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CustomersEditor::createSqlInsert() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CustomersEditor::setSaveData() {
  qDebug() << Q_FUNC_INFO << "TODO";
  /*
    if (c_id->value().toString().isEmpty()) {
      createSqlInsert();
      return;
    }
    createSqlUpdate();
  */
}

void CustomersEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    QStringList info(tr("Unsaved Changes"));
    info << tr("Don't leave this page before save your changes!");
    openNoticeMessage(info.join("\n"));
    return;
  }
  setFinalLeaveEditor();
}

void CustomersEditor::setFinalLeaveEditor() {
  setResetInputFields();
  emit sendLeaveEditor(); /**< Zurück zur Hauptsansicht */
}

void CustomersEditor::setRestore() {
  importSqlResult();
  setEnabled(true);
}

bool CustomersEditor::openEditEntry(qint64 cutomerId) {
  bool status = false;
  if (cutomerId < 1)
    return status;

  setInputFields();

  QString cId = QString::number(cutomerId);
  QString table = m_tableData->tableName();
  QString sql("SELECT *, concat_ws(' ',c_firstname,c_lastname) AS fullname");
  sql.append(" FROM " + table + " WHERE c_id=" + cId + " LIMIT 1;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() != 0) {
    QSqlRecord r = m_tableData->record();
    while (q.next()) {
      foreach (QString key, inputFields) {
        m_tableData->setValue(key, q.value(r.indexOf(key)));
      }
    }
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    importSqlResult();
    setEnabled(true);
  }

  return status;
}

bool CustomersEditor::createNewEntry() {
  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);
  return true;
}
