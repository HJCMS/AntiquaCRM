// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_BOOKSTABLEMODEL_H
#define ANTIQUACRM_BOOKSTABLEMODEL_H

#include <QObject>
#include <QWidget>
#include <AntiquaCRM>

/**
 * @brief Books Table model
 * @ingroup _books
 */
class BooksTableModel final : public AntiquaCRM::ASqlQueryModel
{
  Q_OBJECT

private:
  QSqlRecord queryRecord;

public:
  explicit BooksTableModel(QObject *parent = nullptr);
  const QMap<int,QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_BOOKSTABLEMODEL_H
