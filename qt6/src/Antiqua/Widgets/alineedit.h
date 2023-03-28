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
 * @class ALineEdit
 * @brief AntiquaCRM Line input edit widget
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ALineEdit final : public QLineEdit {
  Q_OBJECT

private:
  QAction *ac_completer;

private Q_SLOTS:
  void showCompleter();

protected Q_SLOTS:
  /**
   * @brief emit sendFocusOut if (text().length()>2)
   */
  void focusOutEvent(QFocusEvent *) override;

  /**
   * @brief prevent dialog acception when return pressed
   */
  void skipReturnPressed();

Q_SIGNALS:
  void sendFocusOut();

public Q_SLOTS:
  /**
   * @brief Set border highlight when unvalid content
   */
  void isValidContent(bool);

public:
  /**
   * @brief AntiquaCRM Line input edit widget
   * @param parent - parent Widget
   */
  explicit ALineEdit(QWidget *parent = nullptr);

  /**
   * @brief Enable+Visible, completer-popup action.
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
