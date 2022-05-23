// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "buchfreund.h"
#include "myicontheme.h"

#include <QHBoxLayout>
#include <QStringList>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>

ProvidersPager::ProvidersPager(QWidget *parent) : QToolBar{parent} {
  setObjectName("inventory_providers_pager");
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  QStyle *wStyle = style();

  btnLast = addAction(tr("Previous"));
  btnLast->setObjectName("pager_button_last");
  btnLast->setIcon(wStyle->standardIcon(QStyle::SP_ArrowLeft));
  btnLast->setEnabled(false);

  current = new QLabel(this);
  current->setObjectName("pager_display");
  current->setText(tr("Providers"));
  current->setAlignment(Qt::AlignCenter);
  addWidget(current);

  btnNext = addAction(tr("Next"));
  btnNext->setObjectName("pager_button_next");
  btnNext->setIcon(wStyle->standardIcon(QStyle::SP_ArrowRight));
  btnNext->setEnabled(false);

  connect(btnLast, SIGNAL(triggered(bool)), this, SLOT(last(bool)));
  connect(btnNext, SIGNAL(triggered(bool)), this, SLOT(next(bool)));
}

void ProvidersPager::next(bool) {
  int c = pages.count();
  if (page != c && c > 0) {
    int n = (page + 1);
    if (n > c) {
      setPage(n);
      page = n;
    }
  }
}

void ProvidersPager::last(bool) {
  int c = pages.count();
  if (page > 0) {
    int n = (page - 1);
    setPage(n);
    page = n;
  }
}

void ProvidersPager::addPages(const QStringList &list) {
  pages.clear();
  int c = 0;
  foreach (QString p, list) {
    pages.insert(c++, p);
  }
  if (pages.count() > 1) {
    btnNext->setEnabled(true);
  }
}

void ProvidersPager::setPage(int index) {
  for (int i = 0; i < pages.count(); i++) {
    if (index == i) {
      current->setText(pages[i]);
      page = i;
      emit pageEntered(page);
      emit pageChanged(current->text());
    }
  }
}

int ProvidersPager::findPage(const QString &find) {
  for (int i = 0; i < pages.count(); i++) {
    if (find == pages[i])
      return i;
  }
  return 0;
}

int ProvidersPager::currentPage() { return page; }

ProvidersToolBar::ProvidersToolBar(QWidget *parent) : QFrame{parent} {
  setObjectName("inventory_providers_toolbar");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("providers_statusbar_layout");

  m_status = new QLabel(this);
  m_status->setStyleSheet("color:red;");
  layout->addWidget(m_status);

  layout->addStretch(1);

  pager = new ProvidersPager(this);
  layout->addWidget(pager);

  btn_order = new QPushButton(myIcon("db_add"), tr("Create Order"), this);
  QString info =
      tr("You must first open the customer before an order can be placed.");
  btn_order->setToolTip(info);
  btn_order->setStatusTip(info);
  btn_order->setEnabled(false);
  layout->addWidget(btn_order);

  btn_refresh = new QPushButton(myIcon("reload"), tr("Refresh"), this);
  layout->addWidget(btn_refresh);

  setLayout(layout);

  connect(btn_order, SIGNAL(clicked()), this, SIGNAL(s_createOrder()));
  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(s_refresh()));
}

void ProvidersToolBar::statusMessage(const QString &msg) {
  m_status->setText(msg);
  QTimer::singleShot((15 * 1000), m_status, SLOT(clear()));
}

void ProvidersToolBar::enableOrderButton(bool b) { btn_order->setEnabled(b); }

void ProvidersToolBar::enterPage(const QString &title) {
  int index = pager->findPage(title);
  if (index >= 0)
    pager->setPage(index);
}

InventoryProviders::InventoryProviders(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers") + "[*]");
  setClosable(false);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_providers_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("providers_stacked_widget");

  bfProvider = new Buchfreund(m_stackedWidget);
  bfProvider->setObjectName("providers_buchfreund");
  m_stackedWidget->insertWidget(0, bfProvider);
  layout->addWidget(m_stackedWidget);

  m_toolBar = new ProvidersToolBar(this);
  layout->addWidget(m_toolBar);

  setLayout(layout);

  initPages();

  connect(m_toolBar, SIGNAL(s_refresh()), bfProvider, SLOT(getOpenOrders()));
  connect(m_toolBar, SIGNAL(s_createOrder()), this, SLOT(createEditOrders()));

  connect(bfProvider->bfDisplay, SIGNAL(s_editCustomer(int)), this,
          SLOT(sendViewCustomer(int)));

  connect(bfProvider->bfDisplay, SIGNAL(s_warning(const QString &)), m_toolBar,
          SLOT(statusMessage(const QString &)));
}

void InventoryProviders::initPages() {
  QStringList list;
  for (int i = 0; i < m_stackedWidget->count(); i++) {
    list.append(m_stackedWidget->widget(i)->windowTitle());
  }
  m_toolBar->pager->addPages(list);
  m_toolBar->enterPage(m_stackedWidget->currentWidget()->windowTitle());
}

void InventoryProviders::sendViewCustomer(int cid) {
  m_toolBar->enableOrderButton(true);
  if (cid > 0) {
    customerId = cid;
    emit openEditCustomer(customerId);
  } else {
    customerId = -1;
  }
}

void InventoryProviders::onEnterChanged() {
  if (m_stackedWidget->currentWidget() == bfProvider) {
    bfProvider->getOpenOrders();
  } else {
    qDebug() << Q_FUNC_INFO << "TODO";
  }
}

void InventoryProviders::createEditOrders() {
  qDebug() << Q_FUNC_INFO << customerId;
  if (customerId < 1)
    return;

  ProviderOrders list = bfProvider->bfDisplay->fetchOrderData();
  ProviderOrders::iterator i;
  for (i = list.begin(); i != list.end(); ++i) {
    ProviderOrder o(*i);
    qDebug() << o.group() << o.param() << o.fieldname() << o.value();
  }

  qDebug() << Q_FUNC_INFO << customerId << list.size();
}
