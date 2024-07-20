// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESEDITOR_H
#define ANTIQUACRM_PLUGIN_STITCHESEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

/**
 * @class StitchesEditor
 * @brief Editor class for all Stitches ...
 * @ingroup TabsPlugin
 */
class ANTIQUACRM_LIBRARY StitchesEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Layout::Row 0
  /**
   * @brief Article number
   */
  AntiquaCRM::SerialId* ip_id;

  /**
   * @brief Article quantity in stock
   */
  AntiquaCRM::CrowdEdit* ip_count;

  /**
   * @brief Bookarticle price
   */
  AntiquaCRM::PriceEdit* ip_price;

  /**
   * @brief Signed Book or not...
   */
  AntiquaCRM::BoolBox* ip_views;

  /**
   * @brief Signed Book or not...
   */
  AntiquaCRM::BoolBox* ip_kolorit;

  /**
   * @brief Signed Book or not...
   */
  AntiquaCRM::BoolBox* ip_landscape;

  /**
   * @brief Year of publication of the book edition
   */
  AntiquaCRM::YearEdit* ip_year;

  /**
   * @brief Restricted for local usage only?
   */
  AntiquaCRM::BoolBox* ip_restricted;

  // Layout::Row 1
  /**
   * @brief Splitter
   * @li Left side, Input fields
   * @li Right side, Thumbnails viewer
   */
  AntiquaCRM::Splitter* m_splitter;

  /**
   * @brief Booktitle
   * @note Restricted to 80 chars
   */
  AntiquaCRM::TextLine* ip_title;

  /**
   * @brief Expanded book title
   */
  AntiquaCRM::TextLine* ip_title_extended;

  /**
   * @brief Book Author
   */
  AntiquaCRM::TextLine* ip_author;

  /**
   * @brief Article condition
   */
  AntiquaCRM::ConditionEdit* ip_condition;

  /**
   * @brief Book-Editor/Publisher
   */
  AntiquaCRM::TextLine* ip_format;

  /**
   * @brief Main Language of this Book
   */
  AntiquaCRM::SelectLanguage* ip_language;

  /**
   * @brief Storage determination
   */
  AntiquaCRM::SelectStorage* ip_storage;

  /**
   * @brief Storage compartment
   */
  AntiquaCRM::TextLine* ip_storage_compartment;

  /**
   * @brief Keywords editor
   */
  AntiquaCRM::KeywordsEdit* ip_keyword;

  // Layout::Row 3
  AntiquaCRM::TabsWidget* m_tabWidget;

  /**
   * @brief Book Description field
   */
  AntiquaCRM::DescripeEditor* ip_description;

  /**
   * @brief Internal Description, local usage only!
   */
  AntiquaCRM::TextField* ip_internal_description;

  /**
   * @brief Date of first entry.
   */
  AntiquaCRM::DateTimeInfo* ip_since;

  /**
   * @brief Last modified date.
   */
  AntiquaCRM::DateTimeInfo* ip_changed;

  /**
   * @brief Standard toolbar for all editor actions.
   */
  AntiquaCRM::TabsEditActionBar* m_actionBar;

  /**
   * @brief Image operations tool bar
   */
  AntiquaCRM::ImageToolBar* m_imageToolBar;

  /**
   * @brief Embedded thumbnail viewer
   */
  AntiquaCRM::ImageThumbnail* m_thumbnail;

  /**
   * @brief Prepare input fields and load the auto-completions.
   * In this function @ref m_tableData must initialed!
   */
  void setInputFields() override;

  /**
   * @brief Fill input fields with SQL query.
   * @param field - SqlQuery » Record » Field » Properties
   * @param value - Value from Database
   */
  bool setDataField(const QSqlField& field, const QVariant& value) override;

  /**
   * @brief Import database query with AntiquaCRM::ASqlDataQuery
   * @note This is the main function to prepare SQL query results.
   * It search with „m_tableData“ for all Input fields and do following:
   *  @li Run Iterator from: m_tableData->getDataset()
   *  @li Fetch Field properties with: m_tableData->getProperties()
   *  @li setDataField with: m_tableData->getProperties()
   *  @li Check ArticleId > 0
   *  @li Set Restore History
   *  @li finally call „setResetModified“
   */
  void importSqlResult() override;

  /**
   * @brief The main SQL query function in this class
   * Send SQL-Statements to AntiquaCRM::ASqlCore::query
   * @li If response data containing „ip_id“ field,
   *    „m_tableData“ and „SerialId“ will modified.
   * @li Additional a Success Message PopUp will also
   *    triggered if no error occurred.
   * @li finally call setResetModified for all Input fields.
   *
   * @param query - PgSQL::Query::Statement
   */
  bool sendSqlQuery(const QString& query) override;

  /**
   * @brief Create Sql dataset from all Inputfields
   * @li Makes a findChildren to all AntiquaCRM::AbstractInput classes.
   * @li Readout the response and check for „ignoreFields“.
   * @li Checking: „isRequired“ and „isValid“ the field.
   * @li If „false“ call openNoticeMessage with Input hints and set Focus,
   *     break the loop function, and response empty QHash!
   * @li On success, response with filled QHash.
   * @note If one of the pointers contain a error, the Hash will cleared!
   */
  const QHash<QString, QVariant> createSqlDataset() override;

  /**
   * @brief Create PgSql UPDATE Statement
   */
  void createSqlUpdate() override;

  /**
   * @brief Create PgSql INSERT Statement
   */
  void createSqlInsert() override;

  /**
   * If the user sets the item inventory to 0!
   * A Pop Up note with article is also thrown out of the order system and the
   * online shops receive a delete order!
   * @note Embedded in setSaveData()!
   */
  bool realyDeactivateEntry();

private Q_SLOTS:
  /**
   * @brief Save Data slot
   * „ip_id“ check decides the caller function!
   * Additional check Article crowd, if to 0 decreased open a PopUp question
   * with realyDeactivateEntry().
   */
  void setSaveData() override;

  /**
   * @brief Some checks before close slot
   * Call „checkIsModified“ and open „unsavedChangesPopup“ if unsaved changes
   * exists!
   * On success call setFinalLeaveEditor()
   */
  void setCheckLeaveEditor() override;

  /**
   * @brief Final leave editor slot
   * In this Method all data fields cleared and polished.
   * Finally the sendLeaveEditor Signal is emitted.
   * @param force - setWindowModified(false)
   */
  void setFinalLeaveEditor(bool force = true) override;

  /**
   * @brief This slot search, add Completers for Storage Compartments
   * The list will called by AntiquaCRM::SelectStorage::getCompartments();
   */
  void setStorageCompartments() override;

  /**
   * @brief Create a BookCard for this Article number
   */
  void setPrintBookCard();

  /**
   * @brief Create a AntiquaCRM::ImageFileSource from Article Id
   */
  void setLoadThumbnail(qint64) override;

  /**
   * @brief Remove Database Thumbnail action ...
   */
  void setRemoveThumbnail(qint64) override;

  /**
   * @brief AntiquaCRM::ImageImportDialog caller
   */
  void setImportEditImage() override;

public Q_SLOTS:
  /**
   * @brief Clear the input fields for new Entry
   */
  void setRestore() override;

public:
  /**
   * @param parent - parent object
   */
  explicit StitchesEditor(QWidget* parent = nullptr);
  ~StitchesEditor() override;

  /**
   * @brief Open Edit Book with Article Number
   * @param articleId - Existing Article number
   * @return true on success otherwise false
   */
  bool openEditEntry(qint64 articleId) override;

  /**
   * @brief Open Editor with default Completers
   * @return success
   */
  bool createNewEntry() override;

  /**
   * @brief Create a csutomized entry
   * @param object
   */
  bool createCustomEntry(const QJsonObject& object) override;
};

#endif // ANTIQUACRM_PLUGIN_STITCHESEDITOR_H
