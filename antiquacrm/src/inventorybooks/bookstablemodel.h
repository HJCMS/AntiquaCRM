// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLEMODEL_H
#define BOOKSTABLEMODEL_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtSql/QSqlQueryModel>

class BooksTableModel : public QSqlQueryModel
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit BooksTableModel(QObject *parent = 0);
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const;

  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // BOOKSTABLEMODEL_H

