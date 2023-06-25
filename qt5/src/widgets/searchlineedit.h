// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SEARCH_LINEEDIT_H
#define ANTIQUACRM_SEARCH_LINEEDIT_H

#include <QLineEdit>
#include <QRegExp>
#include <QMetaType>
#include <QObject>
#include <QRegExpValidator>
#include <QWidget>

/**
 * @brief The SearchLineEdit class
 * @ingroup widgets
 */
class SearchLineEdit : public QLineEdit {
  Q_OBJECT

protected:
  const QRegExpValidator *setTextValidator();
  const QRegExpValidator *setNumericValidator();
  const QRegExpValidator *setArticleValidator();

public:
  enum Validator {
    Strings = 0x1, /**< @brief Strings */
    Numeric = 0x2, /**< @brief Numeric Input */
    Article = 0x4  /**< @brief Numeric and , list */
  };
  explicit SearchLineEdit(QWidget *parent = nullptr);

  void setValidation(SearchLineEdit::Validator);

  static const QRegExp spacePattern();

  int getLength();

  virtual const QString getSearch() = 0;
};

Q_DECLARE_METATYPE(SearchLineEdit::Validator)

#endif // ANTIQUACRM_SEARCH_LINEEDIT_H
