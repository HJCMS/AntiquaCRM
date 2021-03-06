// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef KEYWORD_TABLEMODEL_H
#define KEYWORD_TABLEMODEL_H

#include <QModelIndex>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QVariant>
#include <QWidget>

class KeywordTableModel : public QSqlTableModel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit KeywordTableModel(const QSqlDatabase &db, QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
};

#endif // KEYWORD_TABLEMODEL_H
