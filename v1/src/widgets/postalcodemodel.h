// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_POSTALCODEMODEL_H
#define ANTIQUACRM_POSTALCODEMODEL_H

#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVariant>

class PostalCodeModel : public QStandardItemModel {
  Q_OBJECT

private:
  QJsonObject p_jsonObject;

public:
  explicit PostalCodeModel(QObject *parent = nullptr);
  const QStringList keys();
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

#endif // ANTIQUACRM_POSTALCODEMODEL_H
