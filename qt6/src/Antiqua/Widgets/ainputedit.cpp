// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ainputedit.h"

#include <ASettings>

namespace AntiquaCRM {

AInputEdit::AInputEdit(QObject *parent) : parent{parent} {}

bool AInputEdit::behaviorSettings(const QString &key, bool standard) {
  if (key.isEmpty())
    return false;

  Q_CHECK_PTR(parent);
  bool b = false;
  AntiquaCRM::ASettings cfg(parent);
  cfg.beginGroup("behavior");
  b = cfg.value(key, standard).toBool();
  cfg.endGroup();
  return b;
}

} // namespace AntiquaCRM
