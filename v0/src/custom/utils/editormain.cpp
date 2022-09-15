// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editormain.h"
#include "applicationclient.h"
#include "messagebox.h"

//#ifndef EDITORMAIN_VERBOSE
//#define EDITORMAIN_VERBOSE 1
//#endif

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStringList>

EditorMain::EditorMain(QWidget *parent) : QWidget{parent} {
  setContentsMargins(5, 5, 5, 5);
  setMinimumSize(800, 550);

  m_sql = new HJCMS::SqlCore(this);
}

bool EditorMain::isInIgnoreList(const QString &key) {
  if (ignoreList.size() < 1 || key.isEmpty())
    return false;

  QStringListIterator it(ignoreList);
  while (it.hasNext()) {
    if (it.next() == key)
      return true;
  }
  return false;
}

void EditorMain::socketStatusMessage(const QString &message) {
  QJsonObject obj;
  obj.insert("receiver", QJsonValue("WINDOW"));
  obj.insert("type", QJsonValue("NOTICE"));
  obj.insert("value", QJsonValue(message));
  QByteArray json = QJsonDocument(obj).toJson(QJsonDocument::Compact);
  if (!json.isNull()) {
    ApplicationClient *m_ipc = new ApplicationClient(this);
    m_ipc->sendMessage(QString::fromLocal8Bit(json));
  }
}

const QVariant EditorMain::findResultValue(const QString &key) {
  QVariant out;
  if (sqlQueryResult.count() < 1)
    return out;

  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataField f = sqlQueryResult.at(i);
    if (f.field() == key) {
      return f.value();
    }
  }
  return out;
}

void EditorMain::resetModified(const QStringList &list) {
  if (list.isEmpty()) {
    qWarning("Empty list");
    return;
  }

  foreach (QString name, list) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
#ifdef EDITORMAIN_VERBOSE
      qDebug() << "setModified" << child->objectName();
#endif
    }
  }
  emit s_isModified(false);
}

bool EditorMain::checkIsModified(const QRegularExpression &pattern) {
  if (!pattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return false;
  }

  QList<QObject *> list =
      findChildren<QObject *>(pattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(list.at(i), "isModified",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {
#ifdef EDITORMAIN_VERBOSE
        qDebug() << "checkIsModified" << list.at(i)->objectName() << b;
#endif
        if (b) {
          emit s_isModified(true);
          return true;
        }
      }
    }
  }
  emit s_isModified(false);
  return false;
}

void EditorMain::clearDataFields(const QRegularExpression &pattern) {
  if (!pattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return;
  }
  QList<QObject *> list =
      findChildren<QObject *>(pattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      QMetaObject::invokeMethod(list.at(i), "reset", Qt::QueuedConnection);
#ifdef EDITORMAIN_VERBOSE
      qDebug() << "reset" << list.at(i);
#endif
    }
  }
}

int EditorMain::sqlErrnoMessage(const QString &code, const QString &error) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_errno_message");
  mbox->setMinimumSize(350, 100);
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "PgSQL Error:" << code << error << Qt::endl;
#endif
  return mbox->failed(code, error);
}

int EditorMain::sqlSuccessMessage(const QString &info, int timeout) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_success_message");
  mbox->setMinimumSize(350, 100);
  socketStatusMessage(info);
  return mbox->success(info, timeout);
}

int EditorMain::sqlNoticeMessage(const QString &info) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_notice_message");
  mbox->setMinimumSize(350, 100);
  return mbox->notice(info);
}

EditorMain::~EditorMain() { sqlQueryResult.clear(); }
