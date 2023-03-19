// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_DATETIMEEDIT_H
#define ANTIQUACRM_INPUT_DATETIMEEDIT_H

#include <AGlobal>
#include <QDate>
#include <QDateTimeEdit>
#include <QObject>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaInput
 * @class ADateTimeEdit
 * @brief AntiquaCRM Date Time Edit widget
 */
class ANTIQUACRM_LIBRARY ADateTimeEdit final : public QDateTimeEdit {
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
   * @brief Primary DateTime edit class
   * @param parent      - parent Widget
   * @param mouseEvents - enable/disable Mouse Wheel events
   */
  explicit ADateTimeEdit(QWidget *parent = nullptr, bool mouseEvents = false);

  /**
   * @brief Switch to setReadOnly(true) and ...
   * remove all InputMethods, disable borderstyle and make bg transparent.
   */
  void setReadOnlyMode();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_DATETIMEEDIT_H
