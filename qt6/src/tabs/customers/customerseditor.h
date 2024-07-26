// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSEDITOR_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSEDITOR_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QLabel>
#include <QObject>

class CustomersFinancial;
class CustomersOrders;

/**
 * @class CustomersEditor
 * @brief Editor class for all Customers ...
 * @ingroup TabsPlugin
 */
class ANTIQUACRM_LIBRARY CustomersEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  // Layout::Row 0
  /**
   * @brief customers number
   */
  AntiquaCRM::SerialId *c_id;
  QLabel *displayName;
  AntiquaCRM::DateTimeInfo *c_since;
  AntiquaCRM::DateTimeInfo *c_changed;

  AntiquaCRM::TabsWidget *m_tabWidget;
  AntiquaCRM::CustomersDataWidget *m_dataWidget;
  CustomersFinancial *m_financialData;
  CustomersOrders *m_ordersTable;

  AntiquaCRM::TabsEditActionBar *m_actionBar;

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
  bool setDataField(const QSqlField &field, const QVariant &value) override;

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
  bool sendSqlQuery(const QString &query) override;

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
   * @brief Einkäufe vom Kunden suchen und einfügen!
   */
  void findPurchases();

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
  void setStorageCompartments() override{/* unused */};

  /**
   * @brief Create a AntiquaCRM::ImageFileSource from Article Id
   */
  void setLoadThumbnail(qint64) override{/* unused */};

  /**
   * @brief Remove Database Thumbnail action ...
   */
  void setRemoveThumbnail(qint64) override{/* unused */};

  /**
   * @brief AntiquaCRM::ImageImportDialog caller
   */
  void setImportEditImage() override{/* unused */};

  void setCreateOrderSignal();

  void setCreateMailMessage(const QString &action);

public Q_SLOTS:
  /**
   * @brief Clear the input fields for new Entry
   */
  void setRestore() override;

public:
  /**
   * @param parent - parent object
   */
  explicit CustomersEditor(QWidget *parent = nullptr);
  ~CustomersEditor() override;

  /**
   * @brief Open Edit Book with Article Number
   * @param id - Existing Article number
   * @return true on success otherwise false
   */
  bool openEditEntry(qint64 id) override;

  /**
   * @brief Open Editor with default Completers
   * @return success
   */
  bool createNewEntry() override;

  /**
   * @brief Kunde öffnen oder aus Json Struktur erstellen
   * @param object - Datenstruktur
   *
   * Optional - Kundendaten aus Dienstleister Daten erstellen!
   * @code
   *  "ACTION", @see CustomersWidget::acceptsCustomActions
   *  "TARGET", "customers_tab"
   *  "CUSTOMER", CUSTOMER_ID || JSON_OBJECT
   *  // JSON_OBJECT ist identisch mit ...
   *  SELECT json_extract_path(pr_order_data::JSON,'customer')
   *    FROM provider_orders LIMIT 1;
   * @endcode
   */
  bool createCustomEntry(const QJsonObject &object) override;
};

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSEDITOR_H
