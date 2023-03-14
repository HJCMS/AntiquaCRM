// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PRINTSSTITCHES_SEARCHLINE_H
#define ANTIQUACRM_PRINTSSTITCHES_SEARCHLINE_H

#include <AntiquaWidgets>
#include <QRegExp>
#include <QObject>
#include <QWidget>

/**
 * @brief Inventory Prints Search line edit
 * @ingroup _prints
 */
class PrintsStitchesSearchLine final : public SearchLineEdit {
  Q_OBJECT

private:
  const QRegExp stripPattern;

public:
  explicit PrintsStitchesSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
  bool isValid(int min = 3);
};

#endif // ANTIQUACRM_PRINTSSTITCHES_SEARCHLINE_H
