// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVEDITOR_H
#define ANTIQUACRM_CDVEDITOR_H

#include <AntiquaImgEdit>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CDVEditor final : public InventoryEditor {
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
  explicit CDVEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_CDVEDITOR_H
