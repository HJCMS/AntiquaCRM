// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTSTABLEMODEL_H
#define PRINTSTABLEMODEL_H

#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtSql/QSqlQueryModel>

class PrintsTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit PrintsTableModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // PRINTSTABLEMODEL_H
