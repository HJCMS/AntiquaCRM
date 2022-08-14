// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERSTABLEMODEL_H
#define COSTUMERSTABLEMODEL_H

#include <QDate>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QSqlQueryModel>

class CustomersTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString displayDate(const QVariant &value) const;
  const QIcon getHeaderIcon(int section) const;

public:
  explicit CustomersTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // COSTUMERSTABLEMODEL_H
