// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editormain.h"

#include <QDebug>

EditorMain::EditorMain(QWidget *parent) : QWidget{parent} {
  setMinimumSize(800, 600);
  setContentsMargins(5, 5, 5, 5);
  m_sql = new HJCMS::SqlCore(this);
}

void EditorMain::resetModified(const QStringList &list) {
  if(list.isEmpty())
  {
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
  if(!pattern.isValid())
  {
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

        // qDebug() << Q_FUNC_INFO << list.at(i)->objectName() << b;
        if (b) {
          return true;
        }
      }
    }
  }
  return false;
}

void EditorMain::clearDataFields(const QRegularExpression &pattern) {
  if(!pattern.isValid())
  {
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

EditorMain::~EditorMain() { sqlQueryResult.clear(); }
