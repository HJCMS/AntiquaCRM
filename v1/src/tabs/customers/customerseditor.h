// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSEDITOR_H
#define ANTIQUACRM_CUSTOMERSEDITOR_H

#include <QObject>
#include <QWidget>
#include <inventoryeditor.h>

class CustomersEditor final : public InventoryEditor {
  Q_OBJECT

private:
  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor() override;

public Q_SLOTS:
  void setRestore() override;

public:
  explicit CustomersEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 customerId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_CUSTOMERSEDITOR_H
