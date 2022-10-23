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

#ifndef INPUT_FIND_OPTS
#define INPUT_FIND_OPTS Qt::FindChildrenRecursively
#endif

InventoryEditor::InventoryEditor(const QString &pattern, QWidget *parent)
    : QWidget{parent}, fieldPattern{pattern} {
  setContentsMargins(0, 0, 0, 0);
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_cfg = new AntiquaCRM::ASettings(this);
  timeoutPopUp = m_cfg->value("popup_timeout", 2).toInt();
  m_bookData = nullptr;
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

  QTimer *m_t = new QTimer(d);
  m_t->setInterval((timeoutPopUp * 1000));
  connect(m_t, SIGNAL(timeout()), d, SLOT(close()));
  m_t->start();
  d->exec();
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

  if (m_bookData != nullptr)
    delete m_bookData;
}
