// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSSTATUSBAR_H
#define ANTIQUACRM_VARIOUSSTATUSBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class VariousStatusBar final : public TabStatusBar {
  Q_OBJECT

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit VariousStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_VARIOUSSTATUSBAR_H
