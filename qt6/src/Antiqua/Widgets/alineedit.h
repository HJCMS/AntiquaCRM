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
 * Some extended Functions with SQL-Field properties, predefined Validators,
 * Completer and visual feedback functions. Also includes the minLength property
 * for SQL table field constraints.
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ALineEdit final : public QLineEdit {
  Q_OBJECT
  Q_PROPERTY(int minLength READ getMinLength WRITE setMinLength NOTIFY
                 sendMinLengthChanged)

private:
  /**
   * @brief Minimum Characters length before a event emitted.
   * Only emit Signals if Input length is greater than ...
   */
  int minLength = 2;

  /**
   * @brief Action icon to open Completer Popup.
   * Only visible if a QCompleter is add and Completers size is greater then 0.
   * See also QLineEdit::addAction
   */
  QAction *ac_completer;

  /**
   * @brief Action icon to note invalid content!
   */
  QAction *ac_invalid;

  /**
   * @short Simple text validator
   * @code
   *  "^\\S{2}.+"
   * @endcode
   */
  static const QRegularExpression textPattern();
  void setTextValidator();

  /**
   * @short Simple numeric validator
   * @code
   *  "^\\d+$"
   * @endcode
   */
  static const QRegularExpression digitPattern();
  void setNumericValidator();

  /**
   * @short AntiquaCRM Articlenumber validator
   * The item number system in Antiqua CRM consists of consecutive numbers.
   * When searching, these may also be entered separated by commas.
   * @code
   *  "^(\\d{1,9}[\\,]?)+$"
   * @endcode
   */
  static const QRegularExpression articlePattern();
  void setArticleValidator();

private Q_SLOTS:
  /**
   * @brief Initial QCompleter and add to Editor
   */
  void showCompleter();

  /**
   * @brief emit sendFocusOut if (text().length()>2)
   */
  void focusOutEvent(QFocusEvent *) override;

  /**
   * @brief prevent dialog acception when return pressed
   */
  void skipReturnPressed();

  /**
   * @brief This slot resetting Tooltip from
   */
  void resetVisualFeedback();

Q_SIGNALS:
  /**
   * @brief Minimum Input Characters length has changed
   */
  void sendMinLengthChanged(int);

  /**
   * @brief This signal emitted after focusOutEvent restriction checks
   */
  void sendFocusOut();

  /**
   * @brief This signal emitted when completer Popup is visible.
   */
  void sendCompleterShown();

public Q_SLOTS:
  /**
   * @brief Set Minimum Characters inout length
   */
  void setMinLength(int);

  /**
   * @brief Set border highlight when unvalid content
   */
  void isValidContent(bool);

  /**
   * @brief Set isValidContent, with timeout!
   * @param timeout in Milliseconds
   * @note Only works if it has a focus!
   */
  void setVisualFeedback(int timeout = 2000);

public:
  /**
   * @brief Predefined Input validator enumeration.
   */
  enum InputValidator {
    NOTHING = 0, /**< @brief No Input Validation (default) */
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
   * @brief the minimum acceptable Characters length.
   */
  int getMinLength();

  /**
   * @brief set predefined input validator
   */
  void setValidation(AntiquaCRM::ALineEdit::InputValidator);

  /**
   * @brief Sets Completer-PopUp action icon to active and visible.
   */
  void setCompleterAction(bool enabled = false);

  /**
   * @brief Set SQL Field Properties.
   * @param prop - QSqlField properties
   * This function use QSqlField properties to set QLineEdit properties. It
   * modifies setMaxLength, setPlaceHolderText and if QSqlField::Required is
   * set, setClearButtonEnabled will change to false.
   */
  void setLineEditProperties(const QSqlField &prop);

  /**
   * @brief current text length
   */
  int length();

  /**
   * @brief Article Number
   * Input is a Article Number? Then returning a Zerofilled Id.
   */
  const QString getArticleNumber();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_LINEEDIT_H
