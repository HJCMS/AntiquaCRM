// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STORAGE_TABLE_H
#define STORAGE_TABLE_H

#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QTableView>
#include <QWidget>

namespace HJCMS {
class SqlCore;
};

class StorageModel;

class StorageHeader final : public QHeaderView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  StorageHeader(QWidget *parent = nullptr);
};

class StorageTable final : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString whereClause;
  HJCMS::SqlCore *m_sql;
  StorageHeader *m_header;
  StorageModel *m_model;

private Q_SLOTS:
  void itemClicked(const QModelIndex &);

Q_SIGNALS:
  void queryMessages(const QString &);

public Q_SLOTS:
  void sqlQuery(const QString &statement);
  void findColumn(const QString &);

public:
  struct RowValues {
    int sl_id;
    QString sl_storage;
    QString sl_identifier;
    QString sl_location;
    qint64 sl_zvab_id;
    QString sl_zvab_name;
    qint64 sl_booklooker_id;
    QString sl_booklooker_name;
  };
  StorageTable(QWidget *parent = nullptr);

Q_SIGNALS:
  void itemChanged(const StorageTable::RowValues &i);
};
Q_DECLARE_METATYPE(StorageTable::RowValues);

#endif // STORAGE_TABLE_H
