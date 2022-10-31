// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabproviders.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

TabProviders::TabProviders(QWidget *parent) : Inventory{"providers_tab", parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers"));
  setWindowIcon(getTabIcon("view_list"));
  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  // m_p1Layout->addWidget();
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("providers_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  // m_editorWidget = new ProviderEditor(m_editorPage);
  // m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End
}

void TabProviders::openStartPage() { setCurrentIndex(0); }

void TabProviders::createSearchQuery(const QString &query) {}

void TabProviders::createNewEntry() {
  //  if (m_editorWidget->createNewEntry()) {
  //    setCurrentWidget(m_editorPage);
  //  }
}

void TabProviders::openEntry(qint64 customerId) {
  if (customerId < 1)
    return;

  if (currentIndex() != 0) {
    QString info(tr("Cannot open this provider.") + "<br>");
    info.append(tr("Because the provider tab is not in overview mode."));
    info.append("<p>");
    info.append(tr("Please save and close all open provider first."));
    info.append("</p>");
    QMessageBox::information(this, tr("Providereditor"), info);
    return;
  }

  //  if (m_editorWidget->openEditEntry(articleId)) {
  //    setCurrentWidget(m_editorPage);
  //  }
}

void TabProviders::onEnterChanged() {
  if (!initialed) {
    // initialed = m_table->setQuery();
    // m_searchBar->setFilter(0);
    setCurrentWidget(m_mainPage);
  }
}
