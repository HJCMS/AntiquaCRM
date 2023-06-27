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
  AntiquaCRM::SerialId *cv_id;           /**< @brief ReadOnly:ArticleID */
  AntiquaCRM::CrowdEdit *cv_count;       /**< @brief Bestandsangabe */
  AntiquaCRM::PriceEdit *cv_price;       /**< @brief Preis */
  AntiquaCRM::YearEdit *cv_year;         /**< @brief Jahresangabe */
  AntiquaCRM::BoolBox *cv_including_vat; /**< @brief inkl. Mehwertsteuer */
  AntiquaCRM::BoolBox *cv_restricted;    /**< @brief Öffentlicher verkauf */

  // Layout::Row:1
  AntiquaCRM::Splitter *m_splitter;         /**< @brief Zweite Reihe */
  AntiquaCRM::TextLine *cv_title;           /**< @brief Title */
  AntiquaCRM::TextLine *cv_title_extended;  /**< @brief Title erweitert */
  AntiquaCRM::TextLine *cv_author;          /**< @brief Artist */
  AntiquaCRM::TextLine *cv_publisher;       /**< @brief Herausgeber */
  AntiquaCRM::SelectMediaType *cv_mtype;    /**< @brief Mediatype */
  AntiquaCRM::ConditionEdit *cv_condition;  /**< @brief Umschreibung */
  AntiquaCRM::TextLine *cv_designation;     /**< @brief Zustandsbeschreibung */
  AntiquaCRM::SelectStorage *cv_storage;    /**< @brief Lagerposition */
  AntiquaCRM::KeywordsEdit *cv_keyword;     /**< @brief Schlüsselwörter */
  AntiquaCRM::IsbnEdit *cv_eangtin;         /**< @brief EAN13/GTIN13 */
  QPushButton *btn_cdread;                  /**< @brief Dialog ReadCD ID's */
  AntiquaCRM::ImageToolBar *m_imageToolBar; /**< @brief Bilder Editieren */
  AntiquaCRM::ImageThumbnail *m_thumbnail;  /**< @brief Bildvorschau */

  // Layout::Row:2
  AntiquaCRM::TabsWidget *m_tabWidget;
  AntiquaCRM::TextField *cv_description; /**< @brief Beschreibung */
  AntiquaCRM::TextField *cv_internal_description;
  AntiquaCRM::DateTimeInfo *cv_since;   /**< @brief Erstellt */
  AntiquaCRM::DateTimeInfo *cv_changed; /**< @brief Geändert */

  // Layout::Row:3
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

  void setReadMediaDialog();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit CDsVinylEditor(QWidget *parent = nullptr);
  ~CDsVinylEditor() override;

  bool openEditEntry(qint64 articleId) override;

  bool createNewEntry() override;
};

#endif // ANTIQUACRM_PLUGIN_CDSVINYLEDITOR_H
