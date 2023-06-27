// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYLEDITOR_H
#define ANTIQUACRM_PLUGIN_CDSVINYLEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class ANTIQUACRM_LIBRARY CDsVinylEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Layout::Row 0
  AntiquaCRM::SerialId *cv_id;
  AntiquaCRM::CrowdEdit *cv_count;

  AntiquaCRM::Splitter *m_splitter;
  AntiquaCRM::ImageToolBar *m_imageToolBar;
  AntiquaCRM::ImageThumbnail *m_thumbnail;
  AntiquaCRM::TabsWidget *m_tabWidget;
  AntiquaCRM::TabsEditActionBar *m_actionBar;

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
  explicit CDsVinylEditor(QWidget *parent = nullptr);
  ~CDsVinylEditor() override;

  bool openEditEntry(qint64 articleId) override;

  bool createNewEntry() override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLEDITOR_H
