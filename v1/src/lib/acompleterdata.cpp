#include "acompleterdata.h"
#include "ashareddatafiles.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace AntiquaCRM {

ACompleterData::ACompleterData(const QString &basename)
    : p_basename{basename} {}

const QStringList ACompleterData::completition(const QString &section) {
  QStringList list;
  AntiquaCRM::ASharedDataFiles dataFiles;
  QJsonDocument doc = dataFiles.getJson(p_basename);
  if (doc.isEmpty()) {
    qWarning("ACompleterData missing '%s'", qPrintable(p_basename));
    return list;
  }
  QJsonArray arr = doc.object().value(p_basename).toArray();
  for (int i = 0; i < arr.count(); i++) {
    QJsonObject obj = arr[i].toObject();
    list << obj.value(section).toString();
  }
  return list;
}

}; // namespace AntiquaCRM
