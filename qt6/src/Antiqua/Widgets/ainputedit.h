// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_INPUTEDIT_H
#define ANTIQUA_WIDGETS_INPUTEDIT_H

#include <AGlobal>
#include <QObject>
#include <QVariant>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY AInputEdit {

private:
  QObject *parent;

protected:
  bool behaviorSettings(const QString &key, bool standard = false);

public Q_SLOTS:
  virtual void setValue(const QVariant &) = 0;

public:
  explicit AInputEdit(QObject *parent);
  virtual const QVariant getValue() = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_INPUTEDIT_H
