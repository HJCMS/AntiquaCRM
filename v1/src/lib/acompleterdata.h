// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ACOMPLETERDATA_H
#define ANTIQUACRM_ACOMPLETERDATA_H

#include <AGlobal>
#include <QString>
#include <QStringList>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARAY ACompleterData final {
private:
  const QString p_basename;

public:
  explicit ACompleterData(const QString &basename);
  const QStringList completition(const QString &section);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ACOMPLETERDATA_H
