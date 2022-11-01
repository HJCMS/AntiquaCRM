// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTATUSBAR_H
#define ANTIQUACRM_BOOKSTATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QObject>
#include <QWidget>

class BookStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_createEntry;
  void setHistoryMenu() override;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool);

public:
  explicit BookStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled();
};

#endif // ANTIQUACRM_BOOKSTATUSBAR_H
