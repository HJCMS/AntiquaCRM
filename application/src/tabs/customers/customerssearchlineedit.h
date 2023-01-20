// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H
#define ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CustomersSearchLineEdit : public SearchLineEdit {
  Q_OBJECT

public:
  explicit CustomersSearchLineEdit(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_CUSTOMERSSEARCHLINEEDIT_H
