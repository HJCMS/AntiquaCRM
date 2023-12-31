// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSEDITOR_H
#define ANTIQUACRM_PLUGIN_BOOKSEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class CatalogSearchButton;

class ANTIQUACRM_LIBRARY BooksEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Layout::Row 0
  AntiquaCRM::SerialId *ib_id;
  AntiquaCRM::CrowdEdit *ib_count;
  AntiquaCRM::PriceEdit *ib_price;
  AntiquaCRM::BoolBox *ib_signed;
  AntiquaCRM::BoolBox *ib_restricted;

  // Layout::Row 1
  AntiquaCRM::NumEdit *ib_edition;
  AntiquaCRM::NumEdit *ib_pagecount;
  AntiquaCRM::NumEdit *ib_weight;
  AntiquaCRM::YearEdit *ib_year;
  AntiquaCRM::NumEdit *ib_volume;

  // Layout::Row 2
  AntiquaCRM::Splitter *m_splitter;
  AntiquaCRM::TextLine *ib_title;
  AntiquaCRM::TextLine *ib_title_extended;
  AntiquaCRM::TextLine *ib_author;
  AntiquaCRM::TextLine *ib_publisher;
  AntiquaCRM::ConditionEdit *ib_condition;
  AntiquaCRM::SelectLanguage *ib_language;
  AntiquaCRM::BookBindingEdit *ib_binding;
  AntiquaCRM::TextLine *ib_designation;
  AntiquaCRM::SelectStorage *ib_storage;
  AntiquaCRM::TextLine *ib_storage_compartment;
  AntiquaCRM::KeywordsEdit *ib_keyword;
  AntiquaCRM::IsbnEdit *ib_isbn;
  CatalogSearchButton *btn_dnbQuery;

  // Layout::Row 3
  AntiquaCRM::TabsWidget *m_tabWidget;
  AntiquaCRM::DescripeEditor *ib_description;
  AntiquaCRM::TextField *ib_internal_description;
  AntiquaCRM::DateTimeInfo *ib_since;
  AntiquaCRM::DateTimeInfo *ib_changed;
  AntiquaCRM::TabsEditActionBar *m_actionBar;
  AntiquaCRM::ImageToolBar *m_imageToolBar;
  AntiquaCRM::ImageThumbnail *m_thumbnail;

  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;
  bool realyDeactivateEntry();

private Q_SLOTS:
  void createDNBSearch();
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void setStorageCompartments() override;
  void setPrintBookCard();
  void setLoadThumbnail(qint64) override;
  void setRemoveThumbnail(qint64) override;
  void setImportEditImage() override;

public Q_SLOTS:
  void setRestore() override;

public:
  explicit BooksEditor(QWidget *parent = nullptr);
  ~BooksEditor() override;
  bool openEditEntry(qint64 articleId) override;
  bool createNewEntry() override;
  bool createCustomEntry(const QJsonObject &object) override;
};

#endif // ANTIQUACRM_PLUGIN_BOOKSEDITOR_H
