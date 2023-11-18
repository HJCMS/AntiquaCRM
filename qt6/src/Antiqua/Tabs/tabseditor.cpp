// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabseditor.h"

#include <AntiquaWidgets>
#include <QDebug>
#include <QJsonDocument>
#include <QMessageBox>
#include <QStringList>
#include <QTimer>

namespace AntiquaCRM {

TabsEditor::TabsEditor(const QString &pattern, QWidget *parent)
    : QWidget{parent}, fieldPattern{pattern} {
  setContentsMargins(0, 0, 0, 0);
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_cfg = new AntiquaCRM::ASettings(this);
  messages_timeout =
      m_cfg->groupValue("window_behavior", "popup_timeout", 1).toInt();
  m_tableData = nullptr;
  m_signalMapper = new QSignalMapper(this);
  connect(m_signalMapper, SIGNAL(mappedString(const QString &)),
          SLOT(checkInputModified(const QString &)));
}

TabsEditor::~TabsEditor() {
  if (inputFields.size() > 0)
    inputFields.clear();

  if (ignoreFields.size() > 0)
    ignoreFields.clear();

  if (m_tableData != nullptr)
    delete m_tableData;

  if (m_signalMapper != nullptr)
    m_signalMapper->deleteLater();
}

AntiquaCRM::ASqlDataQuery *TabsEditor::initTableData(const QString &tablename) {
  QString _table = tablename.trimmed().toLower();
  if (_table.isEmpty()) {
    qWarning("Can't load Table data, invalid Table name!");
    return nullptr;
  }

  m_tableData = new AntiquaCRM::ASqlDataQuery(tablename);
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }
  return m_tableData;
}

const QJsonObject TabsEditor::loadSqlConfig(const QString &group) {
  if (group.isEmpty())
    return QJsonObject();

  AntiquaCRM::ASqlFiles _sf("query_tab_config");
  if (!_sf.openTemplate())
    return QJsonObject();

  _sf.setWhereClause("cfg_group='" + group + "'");
  QSqlQuery _q = m_sql->query(_sf.getQueryContent());
  if (_q.size() == 1) {
    _q.next();
    QJsonDocument _d = QJsonDocument::fromJson(_q.value(0).toByteArray());
    if (!_d.object().isEmpty())
      return _d.object();
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << group << m_sql->lastError();
  }
#endif
  return QJsonObject();
}

bool TabsEditor::registerInputChanged() {
  QListIterator<AntiquaCRM::AInputWidget *> it(getInputEditList(fieldPattern));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *m_inp = it.next();
    if (m_inp == nullptr || m_inp->objectName().isEmpty())
      continue;

    connect(m_inp, SIGNAL(sendInputChanged()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(m_inp, m_inp->objectName());
  }
  return true;
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
  AntiquaCRM::AInputWidget *obj = getInputEdit(name);
  if (obj != nullptr)
    return obj->getValue().toLongLong();

  return -1;
}

AntiquaCRM::AInputWidget *TabsEditor::getInputEdit(const QString &name) {
  return findChild<AntiquaCRM::AInputWidget *>(name,
                                               Qt::FindChildrenRecursively);
}

QList<AntiquaCRM::AInputWidget *>
TabsEditor::getInputEditList(const QRegularExpression &pattern) {
  return findChildren<AntiquaCRM::AInputWidget *>(pattern,
                                                  Qt::FindChildrenRecursively);
}

const QVariant TabsEditor::getDataValue(const QString &name) {
  AntiquaCRM::AInputWidget *obj = getInputEdit(name);
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
    AntiquaCRM::AInputWidget *child = getInputEdit(name);
    if (child != nullptr) {
      child->setWindowModified(false);
    }
  }
  setWindowModified(false);
}

void TabsEditor::setProperties(const QString &objectName, QSqlField &field) {
  if (objectName.isEmpty() || !field.isValid())
    return;

  AntiquaCRM::AInputWidget *obj = getInputEdit(objectName);
  if (obj != nullptr) {
    obj->setRestrictions(field);
  }
}

bool TabsEditor::checkIsModified() {
  if (!fieldPattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return false;
  }

  QList<AntiquaCRM::AInputWidget *> list = getInputEditList(fieldPattern);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr && list.at(i)->isWindowModified()) {
      setWindowModified(true);
      return true; // aussteigen
    }
  }
  setWindowModified(false);
  return false;
}

bool TabsEditor::isModifiedCompare(const QString &name,
                                   const QVariant &origin) {
  AntiquaCRM::AInputWidget *_input = getInputEdit(name);
  if (_input == nullptr)
    return false;

  // #ifdef ANTIQUA_DEVELOPEMENT
  //   qDebug() << Q_FUNC_INFO << name << origin;
  // #endif

  QVariant _value = _input->getValue();
  bool _status = false;
  switch (origin.metaType().id()) {
  case QMetaType::QString: {
    QString _buffer = origin.toString();
    int _index = _buffer.compare(_value.toString());
    _status = (_index != 0);
    break;
  }

  case QMetaType::Int:
  case QMetaType::UInt:
  case QMetaType::Long:
  case QMetaType::ULong:
  case QMetaType::LongLong:
  case QMetaType::ULongLong:
    _status = (origin.toInt() != _value.toInt());
    break;

  case QMetaType::Float:
    _status = (origin.toFloat() != _value.toFloat());
    break;

  case QMetaType::Double:
    _status = (origin.toDouble() != _value.toDouble());
    break;

  case QMetaType::Bool:
    _status = (origin.toBool() != _value.toBool());
    break;

  case QMetaType::QDate:
    return (origin.toDate() != _value.toDate());
    break;

  case QMetaType::QTime:
    _status = (origin.toTime() != _value.toTime());
    break;

  case QMetaType::QDateTime:
    _status = (origin.toDateTime() != _value.toDateTime());
    break;

  default:
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "TabsEditor::Type::Missmatch" << name << _value << origin;
#endif
    _status = false;
    break;
  };

  _input->setWindowModified(_status);
  return _status;
}

void TabsEditor::openErrnoMessage(const QString &info, const QString &error) {
  AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
  d->setErrorMessage(info, error);
  d->exec();
  d->deleteLater();
}

void TabsEditor::openSuccessMessage(const QString &info) {
  AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
  d->setSuccessMessage(info);

  QTimer *m_t = new QTimer(d);
  m_t->setInterval((messages_timeout * 1000));
  connect(m_t, SIGNAL(timeout()), d, SLOT(close()));
  m_t->start();
  d->exec();
  d->deleteLater();
}

void TabsEditor::openNoticeMessage(const QString &info) {
  AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
  d->setNoticeMessage(info);
  d->exec();
  d->deleteLater();
}

void TabsEditor::checkInputModified(const QString &name) {
  AntiquaCRM::AInputWidget *m_inp = getInputEdit(name);
  if (m_inp == nullptr)
    return;

  if (m_tableData != nullptr) {
    QVariant _buffer;
    QHashIterator<QString, QVariant> it(m_tableData->getDataset());
    while (it.hasNext()) {
      it.next();
      QSqlField field = m_tableData->getProperties(it.key());
      if (field.name() == name) {
        _buffer = it.value();
        break;
      }
    }

    if (!_buffer.isNull() && isModifiedCompare(name, _buffer)) {
      m_inp->setWindowModified(true);
      setWindowModified(true);
      return;
    }
  }
  m_inp->setWindowModified(false);
  setWindowModified(false);
}

void TabsEditor::unsavedChangesPopup() {
  AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
  d->setUnsavedMessage(
      tr("Do not leave this page until you have saved your changes!"));
  d->exec();
  d->deleteLater();
}

void TabsEditor::pushStatusMessage(const QString &message) {
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

  QList<AntiquaCRM::AInputWidget *> list = getInputEditList(fieldPattern);
  for (int i = 0; i < list.size(); ++i) {
    AntiquaCRM::AInputWidget *obj = list.at(i);
    if (obj != nullptr)
      obj->reset();
  }
  setWindowModified(false);
}

} // namespace AntiquaCRM
