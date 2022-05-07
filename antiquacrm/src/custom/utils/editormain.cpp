// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editormain.h"
#include "applicationclient.h"
#include "messagebox.h"

#ifndef EDITOR_SHOW_DEBUG
#define EDITOR_SHOW_DEBUG false
#endif

#include <QDebug>

EditorMain::EditorMain(QWidget *parent) : QWidget{parent} {
  setMinimumSize(800, 600);
  setContentsMargins(5, 5, 5, 5);

  m_sql = new HJCMS::SqlCore(this);

  m_ipc = new ApplicationClient(this);
}

void EditorMain::socketStatusMessage(const QString &message) {
  m_ipc->sendMessage(message);
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
    }
  }
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

        if (EDITOR_SHOW_DEBUG) {
          qDebug() << "checkIsModified" << list.at(i)->objectName() << b;
        }
        if (b) {
          return true;
        }
      }
    }
  }
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
    }
  }
}

int EditorMain::sqlErrnoMessage(const QString &code, const QString &error) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_errno_message");
  if (EDITOR_SHOW_DEBUG) {
    qDebug() << "PgSQL Errno:" << code << error << Qt::endl;
  }
  return mbox->failed(code, error);
}

int EditorMain::sqlSuccessMessage(const QString &info, int timeout) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_success_message");
  socketStatusMessage(info);
  return mbox->success(info, timeout);
}

int EditorMain::sqlNoticeMessage(const QString &info) {
  MessageBox *mbox = new MessageBox(this);
  mbox->setObjectName("sql_notice_message");
  return mbox->notice(info);
}

EditorMain::~EditorMain() {
  sqlQueryResult.clear();
  if (m_socketClient != nullptr)
    m_socketClient->deleteLater();
}
