// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
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
 * @class ADateTimeEdit
 * @brief AntiquaCRM Date Time Edit widget
 * @ingroup EditInputs
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

public Q_SLOTS:
  void setSqlTimestamp(const QString &);

public:
  /**
   * @param parent      - parent Widget
   * @param mouseEvents - enable/disable Mouse Wheel events
   */
  explicit ADateTimeEdit(QWidget *parent = nullptr, bool mouseEvents = false);

  /**
   * @brief Turn on read-only display mode.
   *
   * This will changed following things:
   *  @li QDateTimeEdit::setInputMethodHints to Qt::ImhNone
   *  @li QDateTimeEdit::setButtonSymbols to QAbstractSpinBox::NoButtons
   *  @li QDateTimeEdit::setReadOnly to true
   *  @li QDateTimeEdit::setFrame to false
   *
   * For Display changes you can use the pseudo state <i>:no-frame</i> or
   * <i>:read-only</i> in Stylesheets.
   * @code
   *  #ADateTimeEdit:no-frame { background:transparent; }
   * @endcode
   */
  void setReadOnlyMode();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_DATETIMEEDIT_H
