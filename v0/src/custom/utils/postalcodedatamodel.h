// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef POSTALCODEDATAMODEL_UTILS_H
#define POSTALCODEDATAMODEL_UTILS_H

#include <QStringListModel>
#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>

namespace HJCMS {
class SqlCore;
};

class PostalCodeDataModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  void fetchTableData();

public:
  explicit PostalCodeDataModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole);
  int rowCount(const QModelIndex &parent = QModelIndex());
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
};

#endif // POSTALCODEDATAMODEL_UTILS_H
