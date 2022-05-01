// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSTABLEMODEL_H
#define ORDERSTABLEMODEL_H

#include <QtCore/QDateTime>
#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQueryModel>

class OrdersTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private Q_SLOTS:
  void update(const QModelIndex &, const QModelIndex &);

Q_SIGNALS:
  void dataUpdated(bool);
  void datealert(int);

public:
  explicit OrdersTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // ORDERSTABLEMODEL_H
