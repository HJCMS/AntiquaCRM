// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTABLEMODEL_H
#define ANTIQUACRM_BOOKSTABLEMODEL_H

#include <QObject>
#include <QWidget>
#include <AntiquaCRM>

class BooksTableModel : public AntiquaCRM::ASqlQueryModel
{
  Q_OBJECT

private:
  QSqlRecord queryRecord;
  const QMap<int,QString> headerList() const override;

public:
  explicit BooksTableModel(QObject *parent = nullptr);
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_BOOKSTABLEMODEL_H
