// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_TEXTEDIT_H
#define ANTIQUACRM_INPUT_TEXTEDIT_H

#include <AGlobal>
#include <QKeyEvent>
#include <QObject>
#include <QRegularExpression>
#include <QTextEdit>

namespace AntiquaCRM {

/**
 * @class ATextEdit
 * @brief AntiquaCRM Text edit widget
 * @note Only Text plain supported for SQL inputs and CSV exports!
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

  /**
   * @brief Pattern for remove SQL injection code
   */
  static const QRegularExpression pattern();

public Q_SLOTS:
  void setText(const QString &);

public:
  /**
   * @brief parent - Parent object
   */
  explicit ATextEdit(QWidget *parent = nullptr);

  const QString strip(const QString &) const;

  const QString text();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_TEXTEDIT_H
