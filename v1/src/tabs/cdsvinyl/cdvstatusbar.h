// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CDVSTATUSBAR_H
#define ANTIQUACRM_CDVSTATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class CDVStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_createEntry;
  void setHistoryMenu() override;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool);

public:
  explicit CDVStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled();
};

#endif // ANTIQUACRM_CDVSTATUSBAR_H
