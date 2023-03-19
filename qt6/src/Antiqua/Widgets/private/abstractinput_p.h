// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_INPUTEDIT_PRIVATE_H
#define ANTIQUA_WIDGETS_INPUTEDIT_PRIVATE_H

#include <AGlobal>
#include <QObject>
#include <QtGlobal>

namespace AntiquaCRM {

class AbstractInput;

class ANTIQUACRM_LIBRARY AbstractInputPrivate {
public:
  bool required_status = false;
  AbstractInputPrivate();
  void init(QWidget *parent);
  ~AbstractInputPrivate();
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_INPUTEDIT_PRIVATE_H
