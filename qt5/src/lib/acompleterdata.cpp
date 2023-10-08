#include "acompleterdata.h"
#include "ashareddatafiles.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

namespace AntiquaCRM {

ACompleterData::ACompleterData(const QString &basename)
    : p_basename{basename} {}

const QJsonObject ACompleterData::getTreeObject() const {
  AntiquaCRM::ASharedDataFiles dataFiles;
  return dataFiles.getJson(p_basename).object();
}

const QStringList ACompleterData::completition(const QStringList &keys) {
  QStringList _output;
  const QJsonObject _tree = getTreeObject();
  if (_tree.isEmpty() || !_tree.contains(p_basename)) {
    qWarning("ACompleterData missing '%s'", qPrintable(p_basename));
    return _output;
  }
  QJsonArray _array = _tree.value(p_basename).toArray();
  for (int i = 0; i < _array.count(); i++) {
    QJsonObject obj = _array[i].toObject();
    QStringList _line;
    foreach (const QString _k, keys) {
      _line << obj.value(_k).toString();
    }
    _output << _line.join(", ");
  }
  return _output;
}

const QStringList ACompleterData::completition(const QString &section) {
  QStringList _output;
  const QJsonObject _tree = getTreeObject();
  if (_tree.isEmpty() || !_tree.contains(p_basename)) {
    qWarning("ACompleterData missing '%s'", qPrintable(p_basename));
    return _output;
  }
  QJsonArray _array = _tree.value(p_basename).toArray();
  for (int i = 0; i < _array.count(); i++) {
    QJsonObject obj = _array[i].toObject();
    _output << obj.value(section).toString();
  }
  return _output;
}

}; // namespace AntiquaCRM
