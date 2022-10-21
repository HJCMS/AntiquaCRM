// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryeditor.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QJsonValue>
#include <QStringList>

InventoryEditor::InventoryEditor(const QString &pattern, QWidget *parent)
    : QWidget{parent}, fieldPattern{pattern} {
  setContentsMargins(0, 0, 0, 0);
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_bookData = nullptr;
}

bool InventoryEditor::isInputField(const QString &fieldName) {
  if (inputList.size() < 1 || fieldName.isEmpty())
    return false;

  QStringListIterator it(inputList);
  while (it.hasNext()) {
    if (it.next() == fieldName)
      return true;
  }
  return false;
}

bool InventoryEditor::isIgnoredField(const QString &fieldName) {
  if (ignoreList.size() < 1 || fieldName.isEmpty())
    return false;

  QStringListIterator it(ignoreList);
  while (it.hasNext()) {
    if (it.next() == fieldName)
      return true;
  }
  return false;
}

void InventoryEditor::setResetModified(const QStringList &list) {
  if (list.isEmpty()) {
    qWarning("Empty list");
    return;
  }
  foreach (QString name, list) {
    InputEdit *child =
        findChild<InputEdit *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
      // qDebug() << "setModified" << child->objectName() << false;
    }
  }
  setWindowModified(false);
}

void InventoryEditor::setProperties(const QString &name, QSqlField &field) {
  if (!field.isValid())
    return;

  InputEdit *obj = findChild<InputEdit *>(name, Qt::FindChildrenRecursively);
  if (obj != nullptr) {
    QMetaObject::invokeMethod(obj, "setProperties", Qt::DirectConnection,
                              Q_ARG(QSqlField, field));
  }
}

bool InventoryEditor::checkIsModified(const QRegularExpression &pattern) {
  if (!pattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return false;
  }

  QList<InputEdit *> list =
      findChildren<InputEdit *>(pattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(list.at(i), "isModified",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {
        // qDebug() << "checkIsModified" << list.at(i)->objectName() << b;
        if (b) {
          setWindowModified(true);
          return true;
        }
      }
    }
  }
  setWindowModified(false);
  return false;
}

void InventoryEditor::openErrnoMessage(const QString &code,
                                       const QString &error) {
  QMessageBox *d = new QMessageBox(this);
  d->setIcon(QMessageBox::Critical);
  d->setDefaultButton(QMessageBox::Ok);
  d->setTextFormat(Qt::PlainText);
  d->setTextInteractionFlags(Qt::TextSelectableByMouse);
  d->setSizeGripEnabled(true);
  d->setWindowTitle(tr("Error"));
  d->setDetailedText(code);
  d->setInformativeText(error);
  d->exec();
}

void InventoryEditor::openSuccessMessage(const QString &info, int timeoutSecs) {
  QMessageBox *d = new QMessageBox(this);
  d->setIcon(QMessageBox::Information);
  d->setDefaultButton(QMessageBox::Ok);
  d->setTextFormat(Qt::PlainText);
  d->setTextInteractionFlags(Qt::TextSelectableByMouse);
  d->setSizeGripEnabled(true);
  d->setWindowTitle(tr("Success"));
  d->setText(info);

  QTimer *m_t = new QTimer(d);
  m_t->setInterval((timeoutSecs * 1000));
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

void InventoryEditor::setClearInputs(const QRegularExpression &pattern) {
  if (!pattern.isValid()) {
    qWarning("Invalid QRegularExpression::objPattern");
    return;
  }
  QList<InputEdit *> list =
      findChildren<InputEdit *>(pattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      QMetaObject::invokeMethod(list.at(i), "reset", Qt::QueuedConnection);
      // qDebug() << "reset" << list.at(i);
    }
  }
}

InventoryEditor::~InventoryEditor() {}
