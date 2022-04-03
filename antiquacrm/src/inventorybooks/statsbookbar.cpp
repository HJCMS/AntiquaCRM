// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsbookbar.h"
#include "version.h"

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>

static const QIcon comboBoxIcon() { return myIcon("info"); }

StatsBookBar::StatsBookBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("StatsBookBar");
  setOrientation(Qt::Horizontal);
  setFloatable(false);

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

  connect(m_showHistory, SIGNAL(currentIndexChanged(int)), this,
          SLOT(historyChanged(int)));
}

/**
EXTRACT(ISODOW FROM ib_changed)=(EXTRACT(ISODOW FROM now()))
EXTRACT(MONTH FROM ib_changed)=(EXTRACT(MONTH FROM now()))
EXTRACT(ISOYEAR FROM ib_changed)=(EXTRACT(YEAR FROM now()))
*/
void StatsBookBar::addComboBoxData() {
  int i = 0;
  m_showHistory->insertItem(i++, comboBoxIcon(), tr("Book data history"), "");
  m_showHistory->insertItem(i++, tr("Today"),"#today");
  m_showHistory->insertItem(i++, tr("Yesterday"), "#yesterday");
  m_showHistory->insertItem(i++, tr("Last 7 Days"), "#last7days");
  m_showHistory->insertItem(i++, tr("This Month"), "#thismonth");
  m_showHistory->insertItem(i++, tr("This Year"), "#thisyear");
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
  if (i > 0 && !m_showHistory->itemData(i, Qt::UserRole).toString().isEmpty())
  {
    emit s_queryHistory(m_showHistory->itemData(i, Qt::UserRole).toString());

    blockSignals(true);
    m_showHistory->setCurrentIndex(0);
    blockSignals(false);
  }
}

void StatsBookBar::showMessage(const QString &str) {
  m_infoLabel->setText(str);
}
