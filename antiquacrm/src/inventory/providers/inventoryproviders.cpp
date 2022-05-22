// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryproviders.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "buchfreund.h"
#include "myicontheme.h"

#include <QtCore>
#include <QtWidgets>

ProvidersToolBar::ProvidersToolBar(QWidget *parent) : QFrame{parent} {
  setObjectName("inventory_providers_toolbar");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("providers_statusbar_layout");

  QLabel *lb = new QLabel(tr("Providers"), this);
  layout->addWidget(lb);

  m_comboBox = new QComboBox(this);
  m_comboBox->setObjectName("provider_selection");
  m_comboBox->setToolTip(tr("set Providers page"));
  m_comboBox->setStatusTip(tr("Current Provider Page"));
  m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  layout->addWidget(m_comboBox);

  layout->addStretch(1);

  btn_order = new QPushButton(myIcon("db_add"), tr("Create Order"), this);
  btn_order->setEnabled(false);
  layout->addWidget(btn_order);

  QPushButton *btn_refresh =
      new QPushButton(myIcon("reload"), tr("Refresh"), this);
  layout->addWidget(btn_refresh);

  setLayout(layout);

  connect(btn_order, SIGNAL(clicked()), this, SIGNAL(s_createOrder()));
  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(s_refresh()));
}

void ProvidersToolBar::addItems(const QStringList &list) {
  m_comboBox->clear();
  m_comboBox->addItems(list);
}

void ProvidersToolBar::setIndex(const QString &title) {
  int index = m_comboBox->findText(title);
  if (index >= 0)
    m_comboBox->setCurrentIndex(index);
}

void ProvidersToolBar::enableOrderButton(bool b) { btn_order->setEnabled(b); }

InventoryProviders::InventoryProviders(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_providers");
  setWindowTitle(tr("Providers") + "[*]");
  setClosable(false);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_providers_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("providers_stacked_widget");

  m_buchfreund = new Buchfreund(m_stackedWidget);
  m_buchfreund->setObjectName("providers_buchfreund");
  m_stackedWidget->insertWidget(0, m_buchfreund);
  layout->addWidget(m_stackedWidget);

  m_toolBar = new ProvidersToolBar(this);
  layout->addWidget(m_toolBar);

  setLayout(layout);

  QStringList list;
  for (int i = 0; i < m_stackedWidget->count(); i++) {
    list.append(m_stackedWidget->widget(i)->windowTitle());
  }
  m_toolBar->addItems(list);
  m_toolBar->setIndex(m_stackedWidget->currentWidget()->windowTitle());

  connect(m_toolBar, SIGNAL(s_refresh()), m_buchfreund, SLOT(getOpenOrders()));
  connect(m_buchfreund, SIGNAL(s_costumer(int)), this,
          SLOT(setButtonState(int)));
}

void InventoryProviders::setButtonState(int cid) {
  if (cid > 0) {
    costumerId = cid;
    m_toolBar->enableOrderButton(true);
  }

  qDebug() << Q_FUNC_INFO << costumerId;
}
