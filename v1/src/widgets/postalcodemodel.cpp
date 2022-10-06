// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodemodel.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QJsonDocument>

//  QJsonArray arr = doc.object().value(country).toArray();
//  for (int i = 0; i < arr.count(); i++) {
//    QJsonObject obj = arr[i].toObject();
//    qDebug() << obj.value("plz");
//  }

PostalCodeModel::PostalCodeModel(QObject *parent) : QStandardItemModel{parent} {
  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson("postalcodes");
  p_jsonObject = QJsonObject();
  if (doc.isEmpty()) {
    qWarning("Postalcodes: JsonDocument is Empty!");
  } else {
    p_jsonObject = doc.object();
  }
  // {"location":"","plz":,"state":""}
  setColumnCount(3);
}

const QStringList PostalCodeModel::keys() {
  QStringList list = p_jsonObject.keys();
  return (list.count() > 0) ? list : QStringList();
}

QVariant PostalCodeModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
    return QVariant();

  if (section == 0)
    return tr("Location");
  else if (section == 1)
    return tr("Postalcode");
  else if (section == 2)
    return tr("State");
  else
    return tr("Unknown");
}
