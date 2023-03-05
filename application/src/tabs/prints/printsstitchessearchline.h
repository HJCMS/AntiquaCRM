// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_SEARCHLINE_H
#define ANTIQUACRM_PRINTSSTITCHES_SEARCHLINE_H

#include <AntiquaWidgets>
#include <QRegExp>
#include <QObject>
#include <QWidget>

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
