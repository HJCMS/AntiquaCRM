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

class LineInputValidator;

/**
 * @class ALineEdit
 * @brief AntiquaCRM Line input edit widget
 * Some extended Functions with SQL-Field properties, predefined Validators,
 * Completer and visual feedback functions. Also includes the minLength property
 * for SQL table field constraints.
 *
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
   * @brief Default input Validator and initialized in constructor.
   * @sa invalidChars
   */
  LineInputValidator *m_validator;

  /**
   * @brief Static List of Invalid Input chars
   *
   * This list is used by Default Input Validator, which is initialized in
   * constructor.
   */
  static const QStringList invalidChars();

  /**
   * @brief Switch to Validator with Text RegEx Pattern
   * @sa setValidation
   */
  void setTextValidator();

  /**
   * @brief Switch to Validator with Numeric RegEx Pattern
   * @sa setValidation
   */
  void setNumericValidator();

  /**
   * @brief Switch to Validator with Article RegEx Pattern
   * @sa setValidation
   */
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
   * @brief Enable Warn icon when unvalid content
   */
  void setIconWarning(bool);

  /**
   * @brief Set border highlight when unvalid content
   */
  void setValidContent(bool);

  /**
   * @brief Set isValidContent, with timeout!
   * @param timeout in Milliseconds
   * @note Only works if it has a focus!
   */
  void setVisualFeedback(int timeout = 1000);

public:
  /**
   * @brief Predefined Input validator enumeration.
   */
  enum InputValidator {
    DEFAULT = 0, /**< @brief Default Validation */
    STRINGS = 1, /**< @brief Match strings */
    NUMERIC = 2, /**< @brief Match numeric */
    ARTICLE = 3, /**< @brief Match ArticleID */
    NOTHING = 4  /**< @brief Disable Validation */
  };

  /**
   * @brief AntiquaCRM Line input edit widget
   * @param parent - parent Widget
   */
  explicit ALineEdit(QWidget *parent = nullptr);

  /**
   * @short Simple Text validator
   * @code
   *  "^\\S{2}.+"
   * @endcode
   */
  static const QRegularExpression textPattern();

  /**
   * @short Simple Numeric validator
   * @code
   *  "^\\d+$"
   * @endcode
   */
  static const QRegularExpression digitPattern();

  /**
   * @short AntiquaCRM Article number validation
   *
   * The item number system in Antiqua CRM consists of consecutive numbers.
   * When searching, these may also be entered separated by commas.
   * @code
   *  "^(\\d{1,9}[\\,]?)+$"
   * @endcode
   */
  static const QRegularExpression articlePattern();

  /**
   * @short AntiquaCRM find TABs and SPACEs
   * @code
   *  "[\\s\\t]+"
   * @endcode
   */
  static const QRegularExpression spacePattern();

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
