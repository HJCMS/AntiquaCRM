// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSSEARCHEDIT_H
#define ANTIQUACRM_VARIOUSSEARCHEDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QRegExp>
#include <QWidget>

/**
 * @brief Inventory Various Search line edit
 * @ingroup _various
 */
class VariousSearchEdit final : public SearchLineEdit {
  Q_OBJECT

private:
  const QRegExp stripPattern;

public:
  explicit VariousSearchEdit(QWidget *parent = nullptr);
  const QString getSearch();
  bool isValid(int min = 3);
};

#endif // ANTIQUACRM_VARIOUSSEARCHEDIT_H
