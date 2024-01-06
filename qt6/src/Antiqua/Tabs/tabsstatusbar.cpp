// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsstatusbar.h"
#include "tabsstatusprogress.h"

#include <AntiquaWidgets>
#include <QDate>
#include <QSizePolicy>

namespace AntiquaCRM {

TabsStatusBar::TabsStatusBar(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 4, 5);

  QHBoxLayout *mainLayout = new QHBoxLayout(this);
  mainLayout->setContentsMargins(4, 0, 0, 0);

  m_status = new TabsStatusProgress(this);
  m_status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  mainLayout->insertWidget(0, m_status, 1);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(0, 0, 0, 0);
  layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);
  m_frame->setLayout(layout);
  mainLayout->insertWidget(1, m_frame);

  btn_refresh = new QPushButton(this);
  btn_refresh->setIcon(antiquaIcon("view-refresh"));
  btn_refresh->setText(tr("Update"));
  btn_refresh->setToolTip(tr("Update current View"));
  btn_refresh->setWhatsThis(tr("When pressed the table query will reloaded."));
  mainLayout->insertWidget(2, btn_refresh);

  setLayout(mainLayout);

  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendReloadView()));
}

TabsStatusBar::~TabsStatusBar() {
  // destroy history menues
  QListIterator<QPushButton *> it(findChildren<QPushButton *>());
  while (it.hasNext()) {
    QPushButton *btn = it.next();
    if (btn->menu() != nullptr)
      btn->menu()->deleteLater();
  }
  // destroy signal mapper
  if (m_mapper != nullptr)
    m_mapper->deleteLater();
}

const QMap<TabsStatusBar::History, QString> TabsStatusBar::historyItems() {
  QMap<TabsStatusBar::History, QString> items;
  items.insert(History::Today, tr("Today"));
  items.insert(History::Yesterday, tr("Yesterday"));
  items.insert(History::LastWeek, tr("Last Week"));
  items.insert(History::LastMonth, tr("Last Month"));
  items.insert(History::ThisWeek, tr("This Week"));
  items.insert(History::ThisMonth, tr("This Month"));
  items.insert(History::ThisYear, tr("This Year"));
  items.insert(History::NOIMAGE, tr("Past Days edited (no Image)"));
  items.insert(History::Customized, tr("Customized"));
  return items;
}

void TabsStatusBar::addButton(QPushButton *btn) { layout->addWidget(btn); }

QPushButton *TabsStatusBar::createButton(const QString &title,
                                         const QString &tip) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(antiquaIcon("database-add"));
  btn->setToolTip(tip);
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("Create"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendCreateEntry()));
  addButton(btn);
  return btn;
}

QPushButton *TabsStatusBar::historyButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(historyIcon());
  btn->setToolTip(tr("History menu"));
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("History"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendDefaultView()));
  addButton(btn);
  return btn;
}

QPushButton *TabsStatusBar::defaultViewButton(const QString &title) {
  QPushButton *btn = new QPushButton(m_frame);
  btn->setIcon(AntiquaCRM::antiquaIcon("x-office-spreadsheet"));
  btn->setToolTip(tr("Push to load the Standard view."));
  btn->setStatusTip(btn->toolTip());
  if (title.isEmpty())
    btn->setText(tr("Default view"));
  else
    btn->setText(title);

  connect(btn, SIGNAL(clicked()), SIGNAL(sendDefaultView()));
  addButton(btn);
  return btn;
}

const QIcon TabsStatusBar::historyIcon() const {
  return AntiquaCRM::antiquaIcon("view-history");
}

void TabsStatusBar::setFilterName(const QString &name) {
  if (name.length() < 3)
    return;

  filterName = name.trimmed();
  emit sendFilterNameChanged(filterName);
}

void TabsStatusBar::setStockName(const QString &name) {
  if (name.length() < 3)
    return;

  stockName = name.trimmed();
  emit sendStockNameChanged(stockName);
}

void TabsStatusBar::setHistoryAction(int index) {
  if (filterName.isEmpty() || stockName.isEmpty()) {
    qWarning("Missing properties: filterName and stockName for "
             "setHistoryAction(int) in TabsStatusBar!");
    return;
  }

  TabsStatusBar::History _history = static_cast<TabsStatusBar::History>(index);
  QString _sql;
  const QString _f(filterName);
  const QDate _cd = QDate::currentDate();
  const QString _stockFilter(" AND " + stockName + ">0");
  const QString _stock = (SearchWithStock ? _stockFilter : "");
  QString _year("DATE_PART('year'," + _f + ")=DATE_PART('year',CURRENT_DATE)");

  switch (_history) {
  case (TabsStatusBar::History::Today): {
    _sql.append("DATE(" + _f + ")=CURRENT_DATE");
    break;
  }

  case (TabsStatusBar::History::Yesterday): {
    _sql.append("DATE(" + _f + ")=(CURRENT_DATE -1)" + _stock);
    break;
  }

  case (TabsStatusBar::History::ThisWeek): {
    _sql.append("DATE_PART('week'," + _f + ")=");
    _sql.append("DATE_PART('week',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastWeek): {
    _sql.append("(DATE(" + _f + ") BETWEEN DATE_TRUNC('weeks', INTERVAL");
    _sql.append(" '-1 weeks' + CURRENT_DATE)::DATE AND CURRENT_DATE)");
    _sql.append(_stock);
    break;
  }

  case (TabsStatusBar::History::ThisMonth): {
    _sql.append("DATE_PART('month'," + _f + ")=");
    _sql.append("DATE_PART('month',CURRENT_DATE)");
    _sql.append(" AND " + _year + _stock);
    break;
  }

  case (TabsStatusBar::History::LastMonth): {
    _sql.append("(DATE(" + _f + ") BETWEEN DATE_TRUNC('months', INTERVAL");
    _sql.append(" '-1 months' + CURRENT_DATE)::DATE AND ");
    _sql.append("MAKE_DATE(" + _cd.toString("yyyy, M, 1") + "))");
    _sql.append(_stock);
    break;
  }

  case (TabsStatusBar::History::ThisYear): {
    _sql.append(_year);
    // Im ersten quartal ist bestand optional danach, bestand erzwingen!
    _sql.append((_cd < QDate(_cd.year(), 4, 1)) ? _stock : _stockFilter);
    break;
  }

  case (TabsStatusBar::History::NOIMAGE): {
    _sql.append("DATE(" + _f + ")>(CURRENT_DATE - 30) ");
    _sql.append(imgFilter + _stockFilter);
    break;
  }

  case (TabsStatusBar::History::Customized): {
    _sql.append("DATE_PART('year'," + _f + ")");
    _sql.append("=(DATE_PART('year', CURRENT_DATE) -1)");
    _sql.append(_stockFilter);
    break;
  }

  default:
    showMessage("No History entry: " + QString::number(index), 5000);
  };

  if (!_sql.isEmpty())
    emit sendHistoryQuery(_sql);
}

void TabsStatusBar::startProgress() {
  m_status->start(tr("Query started, waiting for response."));
}

void TabsStatusBar::finalizeProgress() { m_status->reset(); }

const QString TabsStatusBar::getFilterName() { return filterName; }

const QString TabsStatusBar::getStockName() { return stockName; }

void TabsStatusBar::showMessage(const QString &message, int timeout) {
  m_status->showMessage(message, timeout);
}

void TabsStatusBar::setStockEnabled(bool b) { SearchWithStock = b; }

} // namespace AntiquaCRM
