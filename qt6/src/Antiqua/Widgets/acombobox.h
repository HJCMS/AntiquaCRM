// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_COMBOBOX_H
#define ANTIQUACRM_INPUT_COMBOBOX_H

#include <AGlobal>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

class ALineEdit;

/**
 * @class AComboBox
 * @brief AntiquaCRM Combobox widget
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY AComboBox final : public QComboBox {
  Q_OBJECT

private:
  /**
   * @brief mouse wheel support enabled or not
   */
  bool wheel_support = false;

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
  explicit AComboBox(QWidget *parent = nullptr, bool mouseEvents = false);

  /**
   * @brief set default text, no item selected.
   */
  static const QString withoutDisclosures();

  /**
   * @brief create a Qt:UserRole value with withoutDisclosures() text.
   */
  void setWithoutDisclosures(const QVariant &data = QVariant());
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_COMBOBOX_H
