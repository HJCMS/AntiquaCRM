// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aguiutils.h"

#include <QDebug>

namespace AntiquaCRM {

const QIcon AGuiUtils::getIcon(const QString &name) {
  QIcon _back("://icons/" + name + ".png");
  // qDebug() << Q_FUNC_INFO << name << _back.isNull();
  return QIcon::fromTheme(name, _back);
}

} // namespace AntiquaCRM
