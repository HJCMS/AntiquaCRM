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
#include <QAction>
#include <QFocusEvent>
#include <QLineEdit>
#include <QObject>
#include <QSqlField>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaInput
 * @class ALineEdit
 * @brief AntiquaCRM Line edit widget
 */
class ANTIQUACRM_LIBRARY ALineEdit final : public QLineEdit {
  Q_OBJECT

private:
  QAction *ac_completer;

private Q_SLOTS:
  void showCompleter();

protected Q_SLOTS:
  /**
   * @brief prevent dialog acception when return pressed
   */
  void skipReturnPressed();

public Q_SLOTS:
  /**
   * @brief Set border highlight when unvalid content
   */
  void isValidContent(bool);

public:
  /**
   * @brief AntiquaCRM LineEdit widget
   * @param parent      - parent Widget
   */
  explicit ALineEdit(QWidget *parent = nullptr);

  /**
   * @brief Set Enable/Visible, Action Completer-Popup.
   */
  void setCompleterAction(bool enabled = false);

  /**
   * @brief Set max Length and PlaceHolder Text by given SQL Field Properties.
   * @param prop - QSqlField properties
   */
  void setLineEditProperties(const QSqlField &prop);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_LINEEDIT_H
