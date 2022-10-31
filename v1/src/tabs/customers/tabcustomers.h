// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABCUSTOMERS_H
#define ANTIQUACRM_TABCUSTOMERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class TabCustomers final : public Inventory {
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
  explicit TabCustomers(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABCUSTOMERS_H
