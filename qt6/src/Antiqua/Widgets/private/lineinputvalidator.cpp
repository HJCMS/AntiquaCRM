// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "lineinputvalidator.h"

namespace AntiquaCRM {

LineInputValidator::LineInputValidator(const QStringList &list, QObject *parent)
    : QValidator{parent}, p_list{list} {
}

QValidator::State LineInputValidator::validate(QString &input, int &pos) const {
  Q_UNUSED(pos);
  if (p_list.size() < 1)
    return QValidator::Acceptable;

  foreach (QString _char, p_list) {
    if (input.contains(_char)) {
      return QValidator::Invalid;
    }
  }
  return QValidator::Acceptable;
}

} // namespace AntiquaCRM
