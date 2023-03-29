// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabseditor.h"

#include <AntiquaWidgets>
#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>

TabsEditor::TabsEditor(const QString &pattern, QWidget *parent)
    : QWidget{parent}, fieldPattern{pattern} {
  setContentsMargins(0, 0, 0, 0);
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_cfg = new AntiquaCRM::ASettings(this);
  timeoutPopUp = m_cfg->value("popup_timeout", 1).toInt();
  m_tableData = nullptr;
}

bool TabsEditor::isInputField(const QString &name) {
  if (inputFields.size() < 1 || name.isEmpty())
    return false;

  QStringListIterator it(inputFields);
  while (it.hasNext()) {
    if (it.next() == name)
      return true;
  }
  return false;
}

qint64 TabsEditor::getSerialID(const QString &name) {
  AntiquaCRM::AbstractInput *obj = getInputEdit(name);
  if (obj != nullptr)
    return obj->getValue().toInt();

  return -1;
}

AntiquaCRM::AbstractInput *TabsEditor::getInputEdit(const QString &name) {
  return findChild<AntiquaCRM::AbstractInput *>(name,
                                                Qt::FindChildrenRecursively);
}

QList<AntiquaCRM::AbstractInput *>
TabsEditor::getInputEditList(const QRegularExpression &pcre) {
  return findChildren<AntiquaCRM::AbstractInput *>(pcre,
                                                   Qt::FindChildrenRecursively);
}

const QVariant TabsEditor::getDataValue(const QString &name) {
  AntiquaCRM::AbstractInput *obj = getInputEdit(name);
  if (obj != nullptr)
    return obj->getValue();

  return QVariant();
}

bool TabsEditor::isIgnoredField(const QString &fieldName) {
  if (ignoreFields.size() < 1 || fieldName.isEmpty())
    return false;

  QStringListIterator it(ignoreFields);
  while (it.hasNext()) {
    if (it.next() == fieldName)
      return true;
  }
  return false;
}

void TabsEditor::setDefaultInput(const QSqlField &field) {
  QSqlField sqlField(field);
  if (!field.defaultValue().isNull()) {
    /** @bug Invalid Datatypes from QsqlField */
    QVariant from = field.defaultValue();
    QVariant value;
    switch (field.metaType().id()) {
    case QMetaType::Bool:
      value = from.toBool();
      break;

    case QMetaType::Int:
    case QMetaType::LongLong:
      value = from.toInt();
      break;

    case QMetaType::Double:
      value = from.toDouble();
      break;

    default: {
      QString buffer = from.toString();
      if (buffer == "CURRENT_TIMESTAMP") {
        sqlField.setMetaType(QMetaType(QMetaType::QDateTime));
        value = QDateTime::currentDateTime();
      } else if (buffer == "CURRENT_DATE") {
        sqlField.setMetaType(QMetaType(QMetaType::QDate));
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

void TabsEditor::setResetModified(const QStringList &objectList) {
  if (objectList.isEmpty()) {
    qWarning("Empty list");
    return;
  }
  foreach (QString name, objectList) {
    AntiquaCRM::AbstractInput *child = getInputEdit(name);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
      // qDebug() << "setModified" << child->objectName() << false;
    }
  }
  setWindowModified(false);
}

void TabsEditor::setProperties(const QString &objectName, QSqlField &field) {
  if (objectName.isEmpty() || !field.isValid())
    return;

  AntiquaCRM::AbstractInput *obj = getInputEdit(objectName);
  if (obj != nullptr) {
    obj->setRestrictions(field);
  }
}

bool TabsEditor::checkIsModified() {
  if (!fieldPattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return false;
  }

  QList<AntiquaCRM::AbstractInput *> list = getInputEditList(fieldPattern);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr && list.at(i)->isWindowModified()) {
      setWindowModified(true);
      return true;
    }
  }
  setWindowModified(false);
  return false;
}

bool TabsEditor::isModifiedCompare(const QString &name,
                                   const QVariant &origin) {
  AntiquaCRM::AbstractInput *obj = getInputEdit(name);
  if (obj == nullptr)
    return false;

  // Der Datensatzwert der vieleicht Neu ist?
  QVariant n_value = obj->getValue();
  // Wenn der Datentype nicht stimmt austeigen!
  if (origin.metaType().id() != n_value.metaType().id())
    return true;

  // Jetzt vergleiche anstellen.
  bool status = false;
  switch (origin.metaType().id()) {
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

  obj->setWindowModified(status);
  return status;
}

void TabsEditor::openErrnoMessage(const QString &info, const QString &error) {
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

void TabsEditor::openSuccessMessage(const QString &info) {
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

void TabsEditor::openNoticeMessage(const QString &info) {
  QString t = tr("Notice");
  QMessageBox *d = new QMessageBox(QMessageBox::Warning, t, info,
                                   QMessageBox::Ok, this, Qt::Popup);
  d->exec();
}

void TabsEditor::unsavedChangesPopup() {
  QString ti = tr("Warning");
  QString txt(tr("Unsaved Changes") + "\n");
  txt.append(tr("Do not leave this page until you have saved your changes!"));
  QMessageBox *d = new QMessageBox(QMessageBox::Warning, ti, txt,
                                   QMessageBox::Ok, this, Qt::Popup);
  d->exec();
}

void TabsEditor::sendStatusMessage(const QString &message) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushStatusBarMessage(message))
    m_sock->close();
}

void TabsEditor::pushPluginOperation(const QJsonObject &obj) {
  if (obj.isEmpty())
    return;

  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(obj))
    m_sock->close();
}

void TabsEditor::setResetInputFields() {
  if (!fieldPattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return;
  }

  QList<AntiquaCRM::AbstractInput *> list = getInputEditList(fieldPattern);
  for (int i = 0; i < list.size(); ++i) {
    AntiquaCRM::AbstractInput *obj = list.at(i);
    if (obj != nullptr)
      obj->reset();
  }
}

TabsEditor::~TabsEditor() {
  if (inputFields.size() > 0)
    inputFields.clear();

  if (ignoreFields.size() > 0)
    ignoreFields.clear();

  if (m_tableData != nullptr)
    delete m_tableData;
}
