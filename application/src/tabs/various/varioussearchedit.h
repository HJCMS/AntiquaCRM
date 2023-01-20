// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSSEARCHEDIT_H
#define ANTIQUACRM_VARIOUSSEARCHEDIT_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class VariousSearchEdit final : public SearchLineEdit {
  Q_OBJECT

public:
  explicit VariousSearchEdit(QWidget *parent = nullptr);
  const QString getSearch();
};

#endif // ANTIQUACRM_VARIOUSSEARCHEDIT_H
