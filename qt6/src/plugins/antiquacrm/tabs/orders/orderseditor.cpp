// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "ordersconfig.h"

OrdersEditor::OrdersEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{ORDERS_SQL_EDITOR_PATTERN, parent} {
  //
}

OrdersEditor::~OrdersEditor() {}

void OrdersEditor::setInputFields() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

bool OrdersEditor::setDataField(const QSqlField &field, const QVariant &value) {
  qDebug() << Q_FUNC_INFO << "TODO" << field << value;
  return false;
}

void OrdersEditor::importSqlResult() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

bool OrdersEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "TODO" << query;
  return false;
}

const QHash<QString, QVariant> OrdersEditor::createSqlDataset() {
  QHash<QString, QVariant> _h;
  qDebug() << Q_FUNC_INFO << "TODO";
  return _h;
}

void OrdersEditor::createSqlUpdate() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::createSqlInsert() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::setSaveData() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::setCheckLeaveEditor() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::setFinalLeaveEditor(bool force) {
  qDebug() << Q_FUNC_INFO << "TODO" << force;
}

void OrdersEditor::setRestore() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

bool OrdersEditor::openEditEntry(qint64 orderId) {
  qDebug() << Q_FUNC_INFO << "TODO" << orderId;
  return false;
}

bool OrdersEditor::createNewEntry() {
  //
  return false;
}
