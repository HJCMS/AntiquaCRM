// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdveditor.h"

#include <QLayout>

CDVEditor::CDVEditor(QWidget *parent)
    : InventoryEditor{"^cv_[a-z_]+\\b$", parent} {
  setObjectName("cdv_editor");
  setWindowTitle(tr("Edit CD or Vinyl"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("cdv_editor_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  setLayout(mainLayout);
}

void CDVEditor::setInputFields() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool CDVEditor::setDataField(const QSqlField &field, const QVariant &value) {
  qDebug() << Q_FUNC_INFO << "TODO" << field << value;
  return false;
}

void CDVEditor::importSqlResult() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool CDVEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "TODO" << query;
  return false;
}

const QHash<QString, QVariant> CDVEditor::createSqlDataset() {
  QHash<QString, QVariant> hash;
  qDebug() << Q_FUNC_INFO << "TODO";
  return hash;
}

void CDVEditor::createSqlUpdate() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVEditor::createSqlInsert() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVEditor::setSaveData() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVEditor::setCheckLeaveEditor() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVEditor::setFinalLeaveEditor() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CDVEditor::setRestore() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool CDVEditor::openEditEntry(qint64 articleId) {
  qDebug() << Q_FUNC_INFO << "TODO" << articleId;
  return false;
}

bool CDVEditor::createNewEntry() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return false;
}
