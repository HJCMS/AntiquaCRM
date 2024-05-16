// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_EDITOR_H
#define ANTIQUACRM_TABS_EDITOR_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QEvent>
#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QRegularExpression>
#include <QSignalMapper>
#include <QSqlField>
#include <QStringList>
#include <QVariant>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsEditor
 * @brief Current tab editor
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsEditor : public QWidget {
  Q_OBJECT

private:
  int messages_timeout = 2; /**< @brief seconds */

  QSignalMapper *m_signalMapper;

protected:
  AntiquaCRM::ASqlCore *m_sql;
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief Regular expression for "findChild|findChildren"
   * The object names of the input data fields should be identical to the table
   * field names in @ref inputFields!
   * @code
   *  In Table "inventory_books" the field prefix is "ib_*".
   *  This results in the following regular expression for the object search:
   *    "^ib_[a-z_]+[0-9]?\\b$"
   * @endcode
   * @note With PostgreSQL, field names are always in lower case!
   */
  const QRegularExpression fieldPattern;

  /**
   * @brief All existing Input fields
   */
  QStringList inputFields;

  /**
   * @brief List of data fields contained in @ref inputFields.
   * However, should not be included in SQL::{UPDATE|INSERT}s!
   */
  QStringList ignoreFields;

  /**
   * @brief This container class contains all sql item data.
   */
  AntiquaCRM::ASqlDataQuery *m_tableData;

  /**
   * @brief Initial container class from table
   *
   * This function initialize and load SQL table data for current tab.
   *
   * Additionally it fills the @ref inputFields Variable.
   *
   * If error occurred, a PopUp  message displayed.
   *
   * @param tablename - Sql Tablename
   */
  AntiquaCRM::ASqlDataQuery *initTableData(const QString &tablename);

  /**
   * @brief Function to load Database configurations for current Tab.
   * @param group - Find SqlField „cfg_group“ from table „antiquacrm_configs“!
   * @warning Please do not use this function within a constructor!
   * The best way to call this function is in „setInputFields()“!
   */
  const QJsonObject loadSqlConfig(const QString &group);

  /**
   * @brief Prepare all inputs and fill @ref inputFields.
   * @warning Please do not use this function within a constructor!
   * The best way to call this function is in „openEditEntry“ and
   * „createNewEntry“, before new data initialized!
   */
  virtual void setInputFields() = 0;

  /**
   * @brief Register AInputWidget::sendInputChanged() from Inputs
   */
  bool registerInputChanged();

  /**
   * @brief Searches for table field name in inputFields
   * @param name - objectName
   */
  bool isInputField(const QString &name);

  /**
   * @brief Find SerialID values with object name.
   * @param name - objectName
   */
  qint64 getSerialID(const QString &name);

  /**
   * @brief get Input edit class
   * @param name - objectName
   */
  AntiquaCRM::AInputWidget *getInputEdit(const QString &name);

  /**
   * @brief get Input edit class list
   * @param pattern - Regular expression
   */
  QList<AntiquaCRM::AInputWidget *>
  getInputEditList(const QRegularExpression &pattern);

  /**
   * @brief get data from AntiquaCRM::AInputWidget::objectName()
   */
  const QVariant getDataValue(const QString &name);

  /**
   * @brief Searches for table field name in @ref ignoreFields.
   */
  bool isIgnoredField(const QString &fieldName);

  /**
   * @brief Set default input field options onLoad.
   * @note Requires AntiquaCRM::ASqlDataQuery
   */
  void setDefaultInput(const QSqlField &);

  /**
   * @brief Fill data field with pointer to object/field name.
   * @param field - Is the SQL array identifier that is the same as
   *                Input::ObjectName.
   * @param value - Current Value
   */
  virtual bool setDataField(const QSqlField &field, const QVariant &value) = 0;

  /**
   * @brief Runs through AntiquaCRM::ASqlDataQuery
   * Find data fields and call setDataField() here.
   */
  virtual void importSqlResult() = 0;

  /**
   * @brief Sends SQL statements to the database.
   */
  virtual bool sendSqlQuery(const QString &query) = 0;

  /**
   * @brief Checks and creates the record fields.
   * In this method, all data fields of InputEdit are queried and written to the
   * hash if successful. The creation depends on several factors and includes
   * the following procedure, which is repeated for each data field.
   * Flushing must be done for error prevention and for querying methods.
   */
  virtual const QHash<QString, QVariant> createSqlDataset() = 0;

  /**
   * @brief Create SQL UPDATE statement!
   */
  virtual void createSqlUpdate() = 0;

  /**
   * @brief Create SQL INSERT statement!
   */
  virtual void createSqlInsert() = 0;

  /**
   * @brief Reset all changes with object names list!
   */
  void setResetModified(const QStringList &objectList);

  /**
   * @brief Set properties for the object!
   */
  void setProperties(const QString &objectName, QSqlField &field);

  /**
   * @brief Uses "fieldPattern" to find modified values.
   */
  bool checkIsModified();

  /**
   * @brief Uses "fieldPattern", finds a data field and compare values.
   * @param name   - Objectname
   * @param origin - Origin (Restore Value)
   */
  bool isModifiedCompare(const QString &name, const QVariant &origin);

protected Q_SLOTS:
  void openErrnoMessage(const QString &info, const QString &error);
  void openSuccessMessage(const QString &info);
  void openNoticeMessage(const QString &info);

  /**
   * @brief used by Signal Mapper
   */
  void checkInputModified(const QString &name);

  /**
   * @brief predefined Pop Up unsaved changes message
   */
  void unsavedChangesPopup();

  /**
   * @brief send a status bar message.
   */
  void pushStatusMessage(const QString &message);

  /**
   * @brief Send article status to service provider.
   */
  void pushPluginOperation(const QJsonObject &obj);

  /**
   * @brief Calls the reset() from input fields!
   */
  void setResetInputFields();

  /**
   * @brief Preparation for the SQL/Save operations.
   */
  virtual void setSaveData() = 0;

  /**
   * @brief Method for performing various exit tests.
   * @note Should always be called before setFinalLeaveEditor!
   */
  virtual void setCheckLeaveEditor() = 0;

  /**
   * @brief Quit/Exit
   * Clean up before leaving!
   * Finally, the Signal sendLeaveEditor() must sent here.
   */
  virtual void setFinalLeaveEditor(bool force = true) = 0;

  virtual void setStorageCompartments() = 0;

  virtual void setLoadThumbnail(qint64) = 0;

  virtual void setRemoveThumbnail(qint64) = 0;

  virtual void setImportEditImage() = 0;

Q_SIGNALS:
  /**
   * @brief Send article number to image editor.
   */
  void sendEditImage(qint64);

  /**
   * @brief Submit article number for upload pictures.
   */
  void sendUploadImage(qint64);

  /**
   * @brief Send Editor Exit
   */
  void sendLeaveEditor();

  /**
   * @brief Depending on the tab, send the id to the parent window!
   */
  void sendEditorAction(qint64);

public Q_SLOTS:
  /**
   * @brief Method for resetting data fields.
   */
  virtual void setRestore() = 0;

public:
  /**
   * @param pattern - Der Reguläre Ausdruck für @ref fieldPattern
   * @param parent
   */
  explicit TabsEditor(const QString &pattern, QWidget *parent = nullptr);

  /**
   * @brief Savelly unload
   */
  virtual ~TabsEditor() = 0;

  /**
   * @brief Einen Artikel zum bearbeiten öffnen.
   * @note Hier wird AntiquaCRM::ASqlDataQuery befüllt!
   */
  virtual bool openEditEntry(qint64) = 0;

  /**
   * @brief Create a new empty entry
   */
  virtual bool createNewEntry() = 0;

  /**
   * @brief Create a new entry with Json object
   */
  virtual bool createCustomEntry(const QJsonObject &) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_EDITOR_H
