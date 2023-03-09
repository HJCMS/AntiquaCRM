// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVSEARCHLINE_H
#define ANTIQUACRM_CDVSEARCHLINE_H

#include <AntiquaWidgets>
#include <QObject>
#include <QRegExp>
#include <QWidget>

/**
 * @brief CD+Vinyl Search line widget
 * @ingroup _cdvinyl
 */
class CDVSearchLine final : public SearchLineEdit {
  Q_OBJECT

private:
  const QRegExp stripPattern;

public:
  explicit CDVSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
  bool isValid(int min = 3);
};

#endif // ANTIQUACRM_CDVSEARCHLINE_H
