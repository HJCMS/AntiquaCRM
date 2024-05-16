// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_TEXTEDIT_H
#define ANTIQUACRM_INPUT_TEXTEDIT_H

#include <AGlobal>
#include <QKeyEvent>
#include <QObject>
#include <QTextEdit>

namespace AntiquaCRM {

/**
 * @class ATextEdit
 * @brief AntiquaCRM Text edit widget
 * Only Text plain supported for SQL inputs and CSV exports!
 * The Following functions or features disabled by default in this Input class:
 *  @li TAB gestures
 *  @li Drag and Drop
 *  @li HTML and Formatted rich text
 *  @li Focus Policy restricted to mouse click focus only
 *
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ATextEdit final : public QTextEdit {
  Q_OBJECT

private:
  /**
   * @brief Prevent Keyboard:TAB inputs!
   * @note CSV export delimiter is TAB!
   */
  void keyPressEvent(QKeyEvent *) override;
  void keyReleaseEvent(QKeyEvent *) override;

public Q_SLOTS:
  /**
   * @brief set plain text
   * @note Contains the strip() function
   */
  void setText(const QString &);

public:
  /**
   * @brief parent - Parent object
   */
  explicit ATextEdit(QWidget *parent = nullptr);

  /**
   * @brief current plain text
   * @note Contains the strip() function
   */
  const QString text();

  /**
   * @brief Remove unwanted Character sets
   * Some simple remove or replacements to prevent SQL and CSV export errors.
   *
   * Remove:
   *  @li double Spaces,
   *  @li Tabs
   *
   * Replace:
   *  @li Quotes with: ’
   *  @li Greater than sign with: »
   *  @li Smaller than sign with: «
   */
  static const QString strip(const QString &);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_TEXTEDIT_H
