// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsactionbar.h"
#include "myicontheme.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QHBoxLayout>
#include <QList>
#include <QObject>
#include <QSpacerItem>
#include <QSqlDatabase>

static const QIcon comboBoxIcon() { return myIcon("info"); }

StatsActionBar::StatsActionBar(QWidget *parent, bool showCreateButton)
    : QToolBar{parent} {
  if (objectName().isEmpty())
    setObjectName("StatsActionBar");

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

  btn_new = new QPushButton(tr("New Entry"), this);
  btn_new->setObjectName("create_button");
  QString btn_info = tr("Create a new entry.");
  btn_info.append("\n");
  btn_info.append(
      tr("This button is only activated when a search has been carried out!"));
  btn_new->setToolTip(btn_info);
  btn_new->setIcon(myIcon("db_add"));
  btn_new->setEnabled(false);
  btn_new->setVisible(showCreateButton);
  addWidget(btn_new);

  connect(btn_new, SIGNAL(clicked()), this, SIGNAL(sendCreateEntry()));

  btn_refresh = new QPushButton(tr("Refresh"), this);
  btn_refresh->setObjectName("refresh_button");
  btn_refresh->setIcon(myIcon("reload"));
  btn_refresh->setShortcut(QKeySequence::Refresh);
  addWidget(btn_refresh);

  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendRefreshView()));

  addSeparator();

  m_showHistory = new QComboBox(this);
  m_showHistory->setObjectName("SelectHistoryComboBox");
  m_showHistory->setToolTip(tr("Show data history from ..."));
  addComboBoxData();
  addWidget(m_showHistory);

  connect(m_showHistory, SIGNAL(currentIndexChanged(int)), this,
          SLOT(historyChanged(int)));

  timerID = startTimer(1000);
}

void StatsActionBar::addComboBoxData() {
  int i = 0;
  QIcon sIcon = myIcon("edit");
  m_showHistory->insertItem(i++, comboBoxIcon(), tr("View history"), "");
  m_showHistory->insertItem(i++, sIcon, tr("Today"), "#today");
  m_showHistory->insertItem(i++, sIcon, tr("Yesterday"), "#yesterday");
  m_showHistory->insertItem(i++, sIcon, tr("Last Week"), "#lastweek");
  m_showHistory->insertItem(i++, sIcon, tr("Last Month"), "#lastmonth");
  m_showHistory->insertItem(i++, sIcon, tr("This Week"), "#thisweek");
  m_showHistory->insertItem(i++, sIcon, tr("This Month"), "#thismonth");
  m_showHistory->insertItem(i++, sIcon, tr("This Year"), "#thisyear");
}

void StatsActionBar::timerEvent(QTimerEvent *t) {
  ++countIDs;
  if (QSqlDatabase::database(ApplSettings::sqlConnectioName()).isValid()) {
    setThisDayHistory();
    killTimer(timerID);
  }
  if (countIDs > timeToLife)
    killTimer(timerID);
}

void StatsActionBar::historyChanged(int i) {
  if (i > 0 && !m_showHistory->itemData(i, Qt::UserRole).toString().isEmpty()) {
    emit sendQueryHistory(m_showHistory->itemData(i, Qt::UserRole).toString());

    blockSignals(true);
    m_showHistory->setCurrentIndex(0);
    blockSignals(false);
  }
}

void StatsActionBar::showMessage(const QString &message, int rows) {
  m_infoLabel->setText(message);
  if (btn_new->isVisible())
    btn_new->setEnabled(rows > 0);
}

void StatsActionBar::showRowCount(int count) {
  QString rows(tr("Rowcounts"));
  rows.append(": ");
  rows.append(QString::number(count));
  showMessage(rows);
}

void StatsActionBar::setThisDayHistory() {
  int i = m_showHistory->findData("#today", Qt::UserRole);
  if (i >= 1)
    m_showHistory->setCurrentIndex(i);
}

void StatsActionBar::setEnableCreateButton(bool b) { btn_new->setEnabled(b); }
