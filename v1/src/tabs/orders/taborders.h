// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABORDERS_H
#define ANTIQUACRM_TABORDERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class TabOrders final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  QScrollArea *m_editorPage;

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 customerId) override;
  void onEnterChanged() override;

public:
  explicit TabOrders(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABORDERS_H
