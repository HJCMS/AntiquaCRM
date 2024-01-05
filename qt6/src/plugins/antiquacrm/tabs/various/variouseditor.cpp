// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variouseditor.h"
#include "variousconfig.h"

VariousEditor::VariousEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{VARIOUS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_various_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  setLayout(mainLayout);

  // Signals
}

VariousEditor::~VariousEditor() {}

void VariousEditor::setInputFields() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool VariousEditor::setDataField(const QSqlField &field,
                                 const QVariant &value) {
  Q_UNUSED(field);
  qDebug() << Q_FUNC_INFO << "TODO" << value;
  return false;
}

void VariousEditor::importSqlResult() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool VariousEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "TODO" << query;
  return false;
}

const QHash<QString, QVariant> VariousEditor::createSqlDataset() {
  QHash<QString, QVariant> _hash;
  qDebug() << Q_FUNC_INFO << "TODO";
  return _hash;
}

void VariousEditor::createSqlUpdate() { qDebug() << Q_FUNC_INFO << "TODO"; }

void VariousEditor::createSqlInsert() { qDebug() << Q_FUNC_INFO << "TODO"; }

void VariousEditor::setSaveData() { qDebug() << Q_FUNC_INFO << "TODO"; }

void VariousEditor::setCheckLeaveEditor() {}

void VariousEditor::setFinalLeaveEditor(bool force) {
  qDebug() << Q_FUNC_INFO << "TODO" << force;
}

void VariousEditor::setStorageCompartments() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void VariousEditor::setLoadThumbnail(qint64) {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void VariousEditor::setRemoveThumbnail(qint64) {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void VariousEditor::setImportEditImage() { qDebug() << Q_FUNC_INFO << "TODO"; }

void VariousEditor::setRestore() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool VariousEditor::openEditEntry(qint64 articleId) {
  qDebug() << Q_FUNC_INFO << "TODO" << articleId;
  return false;
}

bool VariousEditor::createNewEntry() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}

bool VariousEditor::createCustomEntry(const QJsonObject &object) {
  qDebug() << Q_FUNC_INFO << "TODO" << object;
  return false;
}
