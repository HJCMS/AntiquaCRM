// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTATUSBAR_H
#define ANTIQUACRM_BOOKSTATUSBAR_H

#include <AntiquaWidgets>
#include <QHash>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

class BookStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *m_newEntry;
  QPushButton *m_selectHistory;
  QSignalMapper *m_historyMapper;
  void setHistoryMenu();

private Q_SLOTS:
  void setHistoryAction(int);

public Q_SLOTS:
  void setCreateButtonEnabled(bool);

public:
  explicit BookStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled();
  static const QHash<TabStatusBar::History, QString> historyItems();
};

#endif // ANTIQUACRM_BOOKSTATUSBAR_H
