// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERSTABLEMODEL_H
#define COSTUMERSTABLEMODEL_H

#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QSqlQueryModel>

class CostumersTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit CostumersTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // COSTUMERSTABLEMODEL_H
