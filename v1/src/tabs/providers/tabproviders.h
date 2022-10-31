// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABPROVIDERS_H
#define ANTIQUACRM_TABPROVIDERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class TabProviders final : public Inventory {
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
  explicit TabProviders(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_TABPROVIDERS_H
