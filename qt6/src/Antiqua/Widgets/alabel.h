// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_LABLE_H
#define ANTIQUACRM_INPUT_LABLE_H

#include <AGlobal>
#include <QLabel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ALabel
 * @brief AntiquaCRM Default Label widget
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ALabel final : public QLabel {
  Q_OBJECT

public:
  explicit ALabel(QWidget *parent = nullptr);
  explicit ALabel(const QString &text, QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_LABLE_H
