// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstoolbar.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QDebug>
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

  pager = new ProvidersPager(this);
  layout->addWidget(pager);

  m_status = new QLabel(this);
  m_status->setStyleSheet("color:red;");
  layout->addWidget(m_status);

  layout->addStretch(1);

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
