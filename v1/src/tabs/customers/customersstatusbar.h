// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSSTATUSBAR_H
#define ANTIQUACRM_CUSTOMERSSTATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

class CustomersStatusBar final : public TabStatusBar {
  Q_OBJECT

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit CustomersStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_CUSTOMERSSTATUSBAR_H
