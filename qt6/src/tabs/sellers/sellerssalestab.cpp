// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerssalestab.h"
#include "sellerssalewidget.h"

#include <QPainter>
#include <QPalette>

SellersSalesTab::SellersSalesTab(QWidget *parent) : QTabWidget{parent} {
  m_tabsBar = new AntiquaCRM::TabsBar(tabBar());
  setTabBar(m_tabsBar);
  setTabPosition(QTabWidget::South);

  connect(m_tabsBar, SIGNAL(tabCloseRequested(int)), SLOT(setTabClosed(int)));
}

const QString SellersSalesTab::infoText() const {
  QStringList _l;
  // Background info when SellersSalesTab is empty
  _l << tr("In this area you can accept orders from your service providers.");
  _l << tr("Select the desired order in the right half to display it here.");
  _l << "" << tr("Only orders are accepted here!");
  _l << tr("For order processing, please see the Orders tab.");
  return _l.join("\n");
}

void SellersSalesTab::paintEvent(QPaintEvent *pe) {
  QTabWidget::paintEvent(pe);
  if (!count()) {
    QPainter p(this);
    p.setBrush(palette().text());
    p.setFont(font());
    p.setOpacity(0.8);
    p.drawText(rect(), Qt::AlignCenter, infoText());
  }
}

void SellersSalesTab::setTabClosed(int index) { removeTab(index); }

SellersSalesWidget *SellersSalesTab::currentPage() {
  return qobject_cast<SellersSalesWidget *>(currentWidget());
}

int SellersSalesTab::addPage(const QString &id, SellersSalesWidget *page) {
  if (page == nullptr || id.isEmpty())
    return -1;

  page->setObjectName(id);
  int index = addTab(page, AntiquaCRM::antiquaIcon("user-properties"), id);
  return index;
}
