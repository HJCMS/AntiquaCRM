// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SEARCH_LINEEDIT_H
#define ANTIQUACRM_SEARCH_LINEEDIT_H

#include <QLineEdit>
#include <QMetaType>
#include <QObject>
#include <QRegExpValidator>
#include <QWidget>

class SearchLineEdit : public QLineEdit {
  Q_OBJECT

protected:
  const QRegExpValidator *setTextValidator();
  const QRegExpValidator *setNumericValidator();
  const QRegExpValidator *setArticleValidator();

public:
  enum Validator {
    Strings = 0x1, /**< Strings */
    Numeric = 0x2, /**< Numeric Input */
    Article = 0x4  /**< Article ID */
  };
  explicit SearchLineEdit(QWidget *parent = nullptr);

  void setValidation(SearchLineEdit::Validator);

  int getLength();
};

Q_DECLARE_METATYPE(SearchLineEdit::Validator)

#endif // ANTIQUACRM_SEARCH_LINEEDIT_H
