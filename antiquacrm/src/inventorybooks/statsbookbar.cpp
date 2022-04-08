// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsbookbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>

static const QIcon comboBoxIcon() { return myIcon("info"); }

StatsBookBar::StatsBookBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("StatsBookBar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);
  timerID = -1;
  countIDs = 0;

  // BEGIN "Messanger Area"
  QWidget *m_info = new QWidget(this);
  QHBoxLayout *m_iLayout = new QHBoxLayout(m_info);

  m_infoLabel = new QLabel(m_info);
  m_infoLabel->setObjectName("SQLQueryTextInfo");
  m_iLayout->addWidget(m_infoLabel);

  m_info->setLayout(m_iLayout);

  QSpacerItem *m_sp =
      new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
  m_iLayout->addSpacerItem(m_sp);

  addWidget(m_info);
  // END "Messanger Area"

  addSeparator();

  m_showHistory = new QComboBox(this);
  m_showHistory->setObjectName("SelectHistoryComboBox");
  m_showHistory->setToolTip(tr("Books data history from ..."));
  addComboBoxData();
  addWidget(m_showHistory);
  addSeparator();

  m_createEntryBtn = new QPushButton(this);
  m_createEntryBtn->setObjectName("CreateNew EntryButton");
  m_createEntryBtn->setText(tr("New Bookentry"));
  m_createEntryBtn->setToolTip(tr("Opens the create new bookentry dialog."));
  m_createEntryBtn->setIcon(myIcon("edit_add"));
  addWidget(m_createEntryBtn);
// Öffnet den Dialog Neuen Bucheintrag erstellen.
  connect(m_createEntryBtn, SIGNAL(clicked()), this,
          SIGNAL(s_createEntryClicked()));

  connect(m_showHistory, SIGNAL(currentIndexChanged(int)), this,
          SLOT(historyChanged(int)));

  // s_createEntryClicked()

  timerID = startTimer(1000);
}

/**
EXTRACT(ISODOW FROM ib_changed)=(EXTRACT(ISODOW FROM now()))
EXTRACT(MONTH FROM ib_changed)=(EXTRACT(MONTH FROM now()))
EXTRACT(ISOYEAR FROM ib_changed)=(EXTRACT(YEAR FROM now()))
*/
void StatsBookBar::addComboBoxData() {
  int i = 0;
  QIcon sIcon = myIcon("edit");
  m_showHistory->insertItem(i++, comboBoxIcon(), tr("Book data history"), "");
  m_showHistory->insertItem(i++, sIcon, tr("Today"), "#today");
  m_showHistory->insertItem(i++, sIcon, tr("Yesterday"), "#yesterday");
  m_showHistory->insertItem(i++, sIcon, tr("Last 7 Days"), "#last7days");
  m_showHistory->insertItem(i++, sIcon, tr("This Month"), "#thismonth");
  m_showHistory->insertItem(i++, sIcon, tr("This Year"), "#thisyear");
}

void StatsBookBar::timerEvent(QTimerEvent *t) {
  ++countIDs;
  if (QSqlDatabase::database(sqlConnectionName).isValid()) {
    setThisDayHistory();
    killTimer(timerID);
  }
  if (countIDs > 30)
    killTimer(timerID);
}

/**
@brief StatsBookBar::historyChanged
@short Erstellt eine Verlaufsanfrage
@code
 SELECT b.ib_id,b.ib_title FROM inventory_books as b
 LEFT JOIN inventory AS i
 WHERE
@endcode
@param i  Index von m_showHistory
*/
void StatsBookBar::historyChanged(int i) {
  if (i > 0 && !m_showHistory->itemData(i, Qt::UserRole).toString().isEmpty()) {
    emit s_queryHistory(m_showHistory->itemData(i, Qt::UserRole).toString());

    blockSignals(true);
    m_showHistory->setCurrentIndex(0);
    blockSignals(false);
  }
}

void StatsBookBar::showMessage(const QString &str) {
  m_infoLabel->setText(str);
}

void StatsBookBar::setThisDayHistory() {
  int i = m_showHistory->findData("#today", Qt::UserRole);
  if (i >= 1)
    m_showHistory->setCurrentIndex(i);
}
