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

  /**
   * @short Simple text validator
   */
  void setTextValidator();

  /**
   * @short Simple numeric validator
   */
  void setNumericValidator();

  /**
   * @short AntiquaCRM Articlenumber validator
   */
  void setArticleValidator();

private Q_SLOTS:
  void showCompleter();

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
  enum InputValidator {
    NOTHING = 0, /**< @brief Not Input Validation (default) */
    STRINGS = 1, /**< @brief Match strings */
    NUMERIC = 2, /**< @brief Match numeric */
    ARTICLE = 3  /**< @brief Match ArticleID */
  };

  /**
   * @brief AntiquaCRM Line input edit widget
   * @param parent - parent Widget
   */
  explicit ALineEdit(QWidget *parent = nullptr);

  /**
   * @brief set Input Validator
   */
  void setValidation(ALineEdit::InputValidator);

  /**
   * @brief Enable+Visible, completer-popup action.
   */
  void setCompleterAction(bool enabled = false);

  /**
   * @brief Set max Length and PlaceHolder Text by given SQL Field Properties.
   * @param prop - QSqlField properties
   */
  void setLineEditProperties(const QSqlField &prop);

  /**
   * @brief current input text length
   */
  int length();
};

} // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::ALineEdit::InputValidator)

#endif // ANTIQUACRM_INPUT_LINEEDIT_H
