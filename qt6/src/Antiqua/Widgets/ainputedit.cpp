// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ainputedit.h"

namespace AntiquaCRM {

AInputEdit::AInputEdit(QObject *parent) : parent{parent} {
  Q_CHECK_PTR(parent);
  m_cfg = new AntiquaCRM::ASettings(parent);
}

bool AInputEdit::behaviorSettings(const QString &key, bool standard) {
  if (key.isEmpty())
    return false;

  bool b = false;
  m_cfg->beginGroup("window_behavior");
  b = m_cfg->value(key, standard).toBool();
  m_cfg->endGroup();
  return b;
}

void AInputEdit::setRequired(bool b) { required_status = b; }

bool AInputEdit::isRequired() { return required_status; }

} // namespace AntiquaCRM
