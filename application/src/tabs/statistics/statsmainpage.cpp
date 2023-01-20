// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsmainpage.h"

#include <AntiquaCRM>
#include <QLabel>
#include <QLayout>

StatsMainPage::StatsMainPage(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  p_startDate = QDate::currentDate();

  QVBoxLayout *layout = new QVBoxLayout(this);
  QString info = tr(
      "On this Page you can select some Chart for Diagnostics and Statistics.");
  QLabel *lb_info = new QLabel(info, this);
  layout->addWidget(lb_info);

  m_chartSelect = new QComboBox(this);
  m_chartSelect->addItem(tr("Without disclosures"), QString());
  m_chartSelect->addItem(tr("Orders per Day in Year"), "DailyOrdersInYear");
  m_chartSelect->addItem(tr("Selled Categories In Year"),
                         "SelledCategoriesInYear");
  m_chartSelect->addItem(tr("Provider Orders in Year"), "ProviderOrdersInYear");
  layout->addWidget(m_chartSelect);

#ifdef ANTIQUA_DEVELOPEMENT
  m_textInfo = new QTextEdit(this);
  m_textInfo->setReadOnly(true);
  QStringList css("background:transparent;");
  css << "border:none;";
  m_textInfo->setStyleSheet("QTextEdit {" + css.join(" ") + "}");
  layout->addWidget(m_textInfo);
#endif

  layout->addStretch(1);

  m_yearSelect = new YearSelect(this);
  m_yearSelect->setValue(p_startDate);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  m_statusBar->addPermanentWidget(m_yearSelect);

  layout->addWidget(m_statusBar);
  setLayout(layout);

  connect(m_yearSelect, SIGNAL(sendYearOpen()), SLOT(createChartAction()));
  connect(m_chartSelect, SIGNAL(currentIndexChanged(int)),
          SLOT(createSelectionInfo(int)));
}

void StatsMainPage::createSelectionInfo(int index) {
  Q_UNUSED(index); // TODO
}

void StatsMainPage::createChartAction() {
  int section = m_chartSelect->currentIndex();
  if (section == 0) {
    m_statusBar->showMessage(tr("No Chart selected!"));
    return;
  }
  QString open = m_chartSelect->itemData(section, Qt::UserRole).toString();
  QDate date = m_yearSelect->value().toDate();
  emit sendOpenChart(open, date);
}

const QDate StatsMainPage::selectedDate() {
  return m_yearSelect->value().toDate();
}
