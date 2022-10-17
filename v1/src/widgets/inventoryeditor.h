// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORY_EDITOR_H
#define ANTIQUACRM_INVENTORY_EDITOR_H

#include <AntiquaCRM>
#include <QHash>
#include <QObject>
#include <QRegularExpression>
#include <QSqlField>
#include <QStringList>
#include <QVariant>
#include <QWidget>

/**
 * @class InventoryEditor
 * @abstract
 * @ingroup widgets
 */
class InventoryEditor : public QWidget {
  Q_OBJECT

protected:
  AntiquaCRM::ASqlCore *m_sql;
  const QRegularExpression fieldPattern;
  QStringList inputList;
  virtual void setInputList() = 0;
  bool isInputField(const QString &fieldName);

  QStringList ignoreList;
  bool isIgnoredField(const QString &fieldName);

  AntiquaCRM::ASqlTable *m_bookTable;

  virtual bool setDataField(const QSqlField &field, const QVariant &value) = 0;
  virtual void importSqlResult() = 0;
  virtual bool sendSqlQuery(const QString &query) = 0;
  virtual const QHash<QString, QVariant> createSqlDataset() = 0;
  virtual void createSqlUpdate() = 0;
  virtual void createSqlInsert() = 0;

  void setResetModified(const QStringList &list);
  void setProperties(const QString &name, QSqlField &field);
  bool checkIsModified(const QRegularExpression &pattern);

protected Q_SLOTS:
  void setClearInputs(const QRegularExpression &pattern);
  virtual void setSaveData() = 0;
  virtual void setCheckLeaveEditor() = 0;
  virtual void setFinalLeaveEditor() = 0;

Q_SIGNALS:
  void sendEditImage(qint64 articleId);
  void sendUploadImage(qint64 articleId);
  void sendArticleChanged(qint64 articleId, int count);
  void sendLeaveEditor();

public Q_SLOTS:
  virtual void setRestore() = 0;

public:
  explicit InventoryEditor(const QString &pattern, QWidget *parent = nullptr);
  virtual bool openEditEntry(qint64 articleId) = 0;
  virtual bool createNewEntry() = 0;
  virtual ~InventoryEditor();
};

#endif // ANTIQUACRM_INVENTORY_EDITOR_H
