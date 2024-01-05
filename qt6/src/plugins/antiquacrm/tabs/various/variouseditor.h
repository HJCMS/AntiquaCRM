// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUSEDITOR_H
#define ANTIQUACRM_PLUGIN_VARIOUSEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ANTIQUACRM_LIBRARY VariousEditor final : public AntiquaCRM::TabsEditor {
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
  void setFinalLeaveEditor(bool force = true) override;
  void setStorageCompartments() override;
  void setLoadThumbnail(qint64) override;
  void setRemoveThumbnail(qint64) override;
  void setImportEditImage() override;

public Q_SLOTS:
  void setRestore() override;

public:
  explicit VariousEditor(QWidget *parent = nullptr);
  ~VariousEditor() override;
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
  bool createCustomEntry(const QJsonObject &object) override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUSEDITOR_H
