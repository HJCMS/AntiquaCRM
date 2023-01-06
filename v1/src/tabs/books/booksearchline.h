// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSEARCHLINE_H
#define ANTIQUACRM_BOOKSEARCHLINE_H

#include <AntiquaWidgets>
#include <QObject>

class BookSearchLine final : public SearchLineEdit {
  Q_OBJECT

public:
  explicit BookSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_BOOKSEARCHLINE_H
