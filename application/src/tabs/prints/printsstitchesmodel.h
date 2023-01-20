// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_MODEL_H
#define ANTIQUACRM_PRINTSSTITCHES_MODEL_H

#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

class PrintsStitchesModel final : public AntiquaCRM::ASqlQueryModel {
  Q_OBJECT

private:
  QSqlRecord queryRecord;

public:
  explicit PrintsStitchesModel(QObject *parent = nullptr);
  const QMap<int, QString> headerList() const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QVariant data(const QModelIndex &item,
                int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_MODEL_H
