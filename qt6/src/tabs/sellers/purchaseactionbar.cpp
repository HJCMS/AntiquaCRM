// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchaseactionbar.h"

PurchaseActionBar::PurchaseActionBar(QWidget* parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(5, 0, 5, 0);

  btn_customer = new QPushButton(tr("View Customer"), this);
  btn_customer->setIcon(AntiquaCRM::antiquaIcon("system-users"));
  btn_customer->setToolTip(tr("Opens Customer in tab „Customer“."));
  addPermanentWidget(btn_customer);

  btn_provider = new QPushButton(tr("Provider Actions"), this);
  btn_provider->setIcon(AntiquaCRM::antiquaIcon("preferences-system"));
  btn_provider->setToolTip(tr("Opens Provider actions dialog."));
  addPermanentWidget(btn_provider);

  btn_create = new QPushButton(tr("Create Order"), this);
  btn_create->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  btn_create->setToolTip(tr("Starts a new assignment in tab „Orders“."));
  btn_create->setEnabled(false);
  addPermanentWidget(btn_create);

  btn_check = new QPushButton(tr("Check Articles"), this);
  btn_check->setIcon(AntiquaCRM::antiquaIcon("view-search"));
  btn_check->setToolTip(tr("Availability search for all items."));
  addPermanentWidget(btn_check);

  connect(btn_customer, SIGNAL(clicked()), SIGNAL(sendViewCustomer()));
  connect(btn_provider, SIGNAL(clicked()), SIGNAL(sendProviderAction()));
  connect(btn_create, SIGNAL(clicked()), SLOT(createClicked()));
  connect(btn_check, SIGNAL(clicked()), SIGNAL(sendCheckArticles()));
}

void PurchaseActionBar::createClicked() {
  emit sendCreateOrder();
  enableCreateButton(false);
}

void PurchaseActionBar::enableCreateButton(bool b) {
  btn_create->setEnabled(b);
}
