// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PRINTSSTITCHES_STATUSBAR_H
#define ANTIQUACRM_PRINTSSTITCHES_STATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QObject>
#include <QWidget>

class PrintsStitchesStatusBar final : public TabStatusBar
{
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit PrintsStitchesStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_STATUSBAR_H
