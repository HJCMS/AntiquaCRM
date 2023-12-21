// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsindex.h"
#include "reportstoolbar.h"

ReportsIndex::ReportsIndex(QWidget *parent)
    : AntiquaCRM::TabsIndex{"reports_tab", parent} {
  setObjectName("reports_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("x-office-spreadsheet"));
  setWindowTitle(getTitle());
  setClosable(true);

  // Begin::MainView
  QWidget *m_mainWidget = new QWidget(this);
  QVBoxLayout *mainLayout = new QVBoxLayout(m_mainWidget);

  m_toolBar = new ReportsToolBar(m_mainWidget);
  mainLayout->addWidget(m_toolBar);

  m_mainWidget->setLayout(mainLayout);
  insertWidget(0, m_mainWidget);
  // End::MainView

  setCurrentIndex(ViewPage::MainView);

  connect(m_toolBar, SIGNAL(signalSelected(const QDate &)),
          SLOT(selectMonth(const QDate &)));
}

void ReportsIndex::setDefaultTableView() {}

void ReportsIndex::selectMonth(const QDate &d) {
    qDebug() << Q_FUNC_INFO << "__TODO__" << d;
}

void ReportsIndex::onEnterChanged() {
  if (initialed)
    return;

  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  initialed = m_toolBar->initSelection(m_sql);
}

const QString ReportsIndex::getTitle() const { return tr("Reports"); }

bool ReportsIndex::customAction(const QJsonObject &) { return false; }

const QStringList ReportsIndex::acceptsCustomActions() const {
  return QStringList();
}
