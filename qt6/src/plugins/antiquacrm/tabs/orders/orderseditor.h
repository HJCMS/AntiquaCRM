// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSEDIT_H
#define ANTIQUACRM_PLUGIN_ORDERSEDIT_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ANTIQUACRM_LIBRARY OrdersEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Layout

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

  void setStorageCompartments() override { /* unused */ };

  void setLoadThumbnail(qint64) override { /* unused */ };

  void setRemoveThumbnail(qint64) override { /* unused */ };

  void setImportEditImage() override { /* unused */ };

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);

  virtual ~OrdersEditor() override;

  bool openEditEntry(qint64 orderId) override;

  bool createNewEntry() override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSEDIT_H
