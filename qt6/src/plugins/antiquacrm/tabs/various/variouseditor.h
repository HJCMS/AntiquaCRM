// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
  // Row:0
  AntiquaCRM::SerialId *va_id;           /**< @brief ReadOnly:ArticleID */
  AntiquaCRM::CrowdEdit *va_count;       /**< @brief Bestandsangabe */
  AntiquaCRM::PriceEdit *va_price;       /**< @brief Preis */
  AntiquaCRM::YearEdit *va_year;         /**< @brief Jahresangabe */
  AntiquaCRM::BoolBox *va_restricted;    /**< @brief Öffentlicher verkauf */

  // Row:1
  AntiquaCRM::Splitter *m_splitter;         /**< @brief Zweite Reihe */
  AntiquaCRM::TextLine *va_title;           /**< @brief Title */
  AntiquaCRM::TextLine *va_title_extended;  /**< @brief Title erweitert */
  AntiquaCRM::ConditionEdit *va_condition;  /**< @brief Umschreibung */
  AntiquaCRM::TextLine *va_designation;     /**< @brief Zustandsbeschreibung */
  AntiquaCRM::SelectStorage *va_storage;    /**< @brief Lagerposition */
  AntiquaCRM::ImageThumbnail *m_imageView;  /**< @brief Vorschau */
  AntiquaCRM::ImageToolBar *m_imageToolBar; /**< @brief Bilder aktionen */

  // Row:2
  AntiquaCRM::TabsWidget *m_tabWidget;       /**< @brief Zusätze */
  AntiquaCRM::TextField *va_description;     /**< @brief Beschreibung */
  AntiquaCRM::TextField *va_int_description; /**< @brief Int. Beschreibung */
  AntiquaCRM::DateTimeInfo *va_since;        /**< @brief Erstellt */
  AntiquaCRM::DateTimeInfo *va_changed;      /**< @brief Geändert */

  // Row:3
  AntiquaCRM::TabsEditActionBar *m_actionBar; /**< @brief Aktionen */

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
  void setStorageCompartments() override{};
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
