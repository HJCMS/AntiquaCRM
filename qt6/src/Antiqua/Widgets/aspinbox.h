// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_SPINBOX_H
#define ANTIQUACRM_INPUT_SPINBOX_H

#include <AGlobal>
#include <QObject>
#include <QSpinBox>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ASpinBox
 * @brief Primary Integer input edit class
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ASpinBox final : public QSpinBox {
  Q_OBJECT

private:
  /**
   * @brief mouse wheel support enabled or not
   */
  bool wheel_support = false;

protected:
  /**
   * @brief disable wheel events ...
   * to prevent unwanted changes on large input masks with scroll widget
   */
  void wheelEvent(QWheelEvent *e) override;

public:
  /**
   * @param parent      - parent Widget
   * @param mouseEvents - enable/disable Mouse Wheel events
   */
  explicit ASpinBox(QWidget *parent = nullptr, bool mouseEvents = false);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_SPINBOX_H
