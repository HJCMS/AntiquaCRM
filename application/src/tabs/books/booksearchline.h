// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_BOOKSEARCHLINE_H
#define ANTIQUACRM_BOOKSEARCHLINE_H

#include <AntiquaWidgets>
#include <QRegExp>
#include <QObject>

/**
 * @brief Books Search line edit
 * @ingroup _books
 */
class BookSearchLine final : public SearchLineEdit {
  Q_OBJECT

private:
  const QRegExp stripPattern;

public:
  explicit BookSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
  bool isValid(int min = 3);
};

#endif // ANTIQUACRM_BOOKSEARCHLINE_H
