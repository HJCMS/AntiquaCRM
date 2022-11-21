// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerpurchasebar.h"

#include <QIcon>

ProviderPurchaseBar::ProviderPurchaseBar(QWidget *parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(5, 0, 5, 0);

  btn_provider = new QPushButton(tr("Provider Actions"), this);
  btn_provider->setIcon(QIcon("://icons/network.png"));
  addPermanentWidget(btn_provider);

  btn_create = new QPushButton(tr("Create Order"), this);
  btn_create->setIcon(QIcon("://icons/action_add.png"));
  btn_create->setEnabled(false);
  addPermanentWidget(btn_create);

  btn_check = new QPushButton(tr("Check Articles"), this);
  btn_check->setIcon(QIcon("://icons/action_search.png"));
  QString info = tr("Create a search query to see if all items are available.");
  btn_check->setToolTip(info);
  addPermanentWidget(btn_check);

  connect(btn_provider, SIGNAL(clicked()), SIGNAL(sendProviderAction()));
  connect(btn_create, SIGNAL(clicked()), SLOT(createClicked()));
  connect(btn_check, SIGNAL(clicked()), SIGNAL(sendCheckArticles()));
}

void ProviderPurchaseBar::createClicked() {
  emit sendCreateOrder();
  enableCreateButton(false);
}

void ProviderPurchaseBar::enableCreateButton(bool b) {
  btn_create->setEnabled(b);
}
