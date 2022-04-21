// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DATATABLEMODEL_H
#define DATATABLEMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

class DataTableModel : public QSqlTableModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QSqlDatabase p_db;
  const QString p_type;
  const QString p_table = QString("ui_autofill_keywords");

private Q_SLOTS:
  bool updateData(const QModelIndex &index, const QVariant &value);

Q_SIGNALS:
  void s_dataChanged(bool b);
  void s_sqlError(const QString &);

public Q_SLOTS:
  void removeData(const QModelIndex &index);

public:
  explicit DataTableModel(const QString &field, QTableView *parent = nullptr,
                          QSqlDatabase db = QSqlDatabase());

  bool insertSqlQuery(const QSqlRecord &rec);

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::DisplayRole);

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // DATATABLEMODEL_H
