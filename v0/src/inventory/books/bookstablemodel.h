// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLEMODEL_H
#define BOOKSTABLEMODEL_H

#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QSqlQueryModel>

class BooksTableModel : public QSqlQueryModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString displayDate(const QVariant &value) const;

public:
  explicit BooksTableModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // BOOKSTABLEMODEL_H
