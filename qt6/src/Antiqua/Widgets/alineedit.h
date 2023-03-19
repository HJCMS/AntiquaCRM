// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_LINEEDIT_H
#define ANTIQUACRM_INPUT_LINEEDIT_H

#include <AGlobal>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaInput
 * @class ALineEdit
 * @brief AntiquaCRM Line edit widget
 */
class ANTIQUACRM_LIBRARY ALineEdit final : public QLineEdit {
  Q_OBJECT

protected Q_SLOTS:
  void skipReturnPressed();

public:
  explicit ALineEdit(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_LINEEDIT_H
