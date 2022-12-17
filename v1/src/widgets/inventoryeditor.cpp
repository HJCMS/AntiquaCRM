// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryeditor.h"

#include <AntiquaWidgets>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>

#ifndef INPUT_FIND_OPTS
#define INPUT_FIND_OPTS Qt::FindChildrenRecursively
#endif

InventoryEditor::InventoryEditor(const QString &pattern, QWidget *parent)
    : QWidget{parent}, fieldPattern{pattern} {
  setContentsMargins(0, 0, 0, 0);
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_cfg = new AntiquaCRM::ASettings(this);
  timeoutPopUp = m_cfg->value("popup_timeout", 1).toInt();
  m_tableData = nullptr;
}

bool InventoryEditor::isInputField(const QString &fieldName) {
  if (inputFields.size() < 1 || fieldName.isEmpty())
    return false;

  QStringListIterator it(inputFields);
  while (it.hasNext()) {
    if (it.next() == fieldName)
      return true;
  }
  return false;
}

qint64 InventoryEditor::getSerialID(const QString &name) {
  SerialID *obj = findChild<SerialID *>(name, INPUT_FIND_OPTS);
  if (obj != nullptr)
    return obj->value().toInt();

  return -1;
}

const QVariant InventoryEditor::getDataValue(const QString &objectName) {
  QVariant retval;
  InputEdit *obj = findChild<InputEdit *>(objectName, INPUT_FIND_OPTS);
  if (obj != nullptr)
    return obj->value();

  return retval;
}

InputEdit *InventoryEditor::getInputEdit(const QString &objectName) {
  return findChild<InputEdit *>(objectName, INPUT_FIND_OPTS);
}

bool InventoryEditor::isIgnoredField(const QString &fieldName) {
  if (ignoreFields.size() < 1 || fieldName.isEmpty())
    return false;

  QStringListIterator it(ignoreFields);
  while (it.hasNext()) {
    if (it.next() == fieldName)
      return true;
  }
  return false;
}

void InventoryEditor::setDefaultInput(const QSqlField &field) {
  QSqlField sqlField(field);
  if (!field.defaultValue().isNull()) {
    /** @bug Invalid Datatypes from QsqlField */
    QVariant from = field.defaultValue();
    QVariant value;
    switch (field.type()) {
    case QVariant::Bool:
      value = from.toBool();
      break;

    case QVariant::Int:
    case QVariant::LongLong:
      value = from.toInt();
      break;

    case QVariant::Double:
      value = from.toDouble();
      break;

    default: {
      QString buffer = from.toString();
      if (buffer == "CURRENT_TIMESTAMP") {
        sqlField.setType(QVariant::DateTime);
        value = QDateTime::currentDateTime();
      } else if (buffer == "CURRENT_DATE") {
        sqlField.setType(QVariant::Date);
        value = QDate::currentDate();
      } else {
        value = buffer;
      }
    } break;
    };
    setDataField(sqlField, value);
  } else {
    setProperties(sqlField.name(), sqlField);
  }
}

void InventoryEditor::setResetModified(const QStringList &objectList) {
  if (objectList.isEmpty()) {
    qWarning("Empty list");
    return;
  }
  foreach (QString name, objectList) {
    InputEdit *child = findChild<InputEdit *>(name, INPUT_FIND_OPTS);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
      // qDebug() << "setModified" << child->objectName() << false;
    }
  }
  setWindowModified(false);
}

void InventoryEditor::setProperties(const QString &objectName,
                                    QSqlField &field) {
  if (objectName.isEmpty() || !field.isValid())
    return;

  InputEdit *obj = findChild<InputEdit *>(objectName, INPUT_FIND_OPTS);
  if (obj != nullptr) {
    obj->setProperties(field);
  }
}

bool InventoryEditor::checkIsModified() {
  if (!fieldPattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return false;
  }

  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, INPUT_FIND_OPTS);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(list.at(i), "isModified",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {

        if (b) {
#ifdef ANTIQUA_DEVELOPEMENT
          qDebug() << "IsModified" << list.at(i)->objectName() << b;
#endif
          setWindowModified(true);
          return true;
        }
      }
    }
  }
  setWindowModified(false);
  return false;
}

bool InventoryEditor::isModifiedCompare(const QString &name,
                                        const QVariant &origin) {
  InputEdit *obj = findChild<InputEdit *>(name, INPUT_FIND_OPTS);
  if (obj == nullptr)
    return false;

  // Der Datensatzwert der vieleicht Neu ist?
  QVariant n_value = obj->value();
  // Wenn der Datentype nicht stimmt austeigen!
  if (origin.type() != n_value.type())
    return true;

  // Datentype nach QMetaType verschieben.
  QMetaType metaType(QMetaType::type(origin.typeName()));

  // Jetzt vergleiche anstellen.
  bool status = false;
  switch (metaType.id()) {
  case QMetaType::QString: {
    QString buffer = origin.toString();
    int index = buffer.compare(n_value.toString());
    status = (index != 0);
    break;
  }

  case QMetaType::Int:
  case QMetaType::UInt:
  case QMetaType::Long:
  case QMetaType::ULong:
  case QMetaType::LongLong:
  case QMetaType::ULongLong:
    status = (origin.toInt() != n_value.toInt());
    break;

  case QMetaType::Float:
    status = (origin.toFloat() != n_value.toFloat());
    break;

  case QMetaType::Double:
    status = (origin.toDouble() != n_value.toDouble());
    break;

  case QMetaType::Bool:
    status = (origin.toBool() != n_value.toBool());
    break;

  case QMetaType::QDate:
    return (origin.toDate() != n_value.toDate());
    break;

  case QMetaType::QTime:
    status = (origin.toTime() != n_value.toTime());
    break;

  case QMetaType::QDateTime:
    status = (origin.toDateTime() != n_value.toDateTime());
    break;

  default:
    status = false;
    break;
  };

  obj->setModified(status);
  return status;
}

void InventoryEditor::openErrnoMessage(const QString &info,
                                       const QString &error) {
  QMessageBox *d = new QMessageBox(this);
  d->setIcon(QMessageBox::Critical);
  d->setDefaultButton(QMessageBox::Ok);
  d->setTextFormat(Qt::PlainText);
  d->setTextInteractionFlags(Qt::TextSelectableByMouse);
  d->setSizeGripEnabled(true);
  d->setWindowTitle(tr("Error"));
  d->setDetailedText(info);
  d->setInformativeText(error);
  d->exec();
}

void InventoryEditor::openSuccessMessage(const QString &info) {
  QMessageBox *d = new QMessageBox(this);
  d->setIcon(QMessageBox::Information);
  d->setDefaultButton(QMessageBox::Ok);
  d->setTextFormat(Qt::PlainText);
  d->setTextInteractionFlags(Qt::TextSelectableByMouse);
  d->setSizeGripEnabled(true);
  d->setWindowTitle(tr("Success"));
  d->setText(info);

  QTimer *m_t = new QTimer(d);
  m_t->setInterval((timeoutPopUp * 1000));
  connect(m_t, SIGNAL(timeout()), d, SLOT(close()));
  m_t->start();
  d->exec();
}

void InventoryEditor::openNoticeMessage(const QString &info) {
  QString t = tr("Notice");
  QMessageBox *d = new QMessageBox(QMessageBox::Warning, t, info,
                                   QMessageBox::Ok, this, Qt::Popup);
  d->exec();
}

void InventoryEditor::sendStatusMessage(const QString &message) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushStatusBarMessage(message))
    m_sock->close();
}

void InventoryEditor::pushPluginOperation(const QJsonObject &obj) {
  if (obj.isEmpty())
    return;

  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(obj))
    m_sock->close();
}

void InventoryEditor::setResetInputFields() {
  if (!fieldPattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return;
  }

  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, INPUT_FIND_OPTS);
  for (int i = 0; i < list.size(); ++i) {
    InputEdit *obj = list.at(i);
    if (obj != nullptr)
      obj->reset();
  }
}

InventoryEditor::~InventoryEditor() {
  if (inputFields.size() > 0)
    inputFields.clear();

  if (ignoreFields.size() > 0)
    ignoreFields.clear();

  if (m_tableData != nullptr)
    delete m_tableData;
}
