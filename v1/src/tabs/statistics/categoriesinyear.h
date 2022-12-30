// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CATEGORIES_YEAR_H
#define ANTIQUACRM_CATEGORIES_YEAR_H

#include <AGlobal>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>

class CategoriesInYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;

public:
  explicit CategoriesInYear(const QDate &date, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_CATEGORIES_YEAR_H
