// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerpurchaseactionbar.h"

#include <QIcon>

ProviderPurchaseActionBar::ProviderPurchaseActionBar(QWidget *parent)
    : QToolBar{parent} {
  setMovable(false);
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  addWidget(new QLabel(tr("Customer Id") + ": "));

  QFontMetricsF fm(font());
  qreal mWidth = fm.horizontalAdvance("999999999");
  m_edit = new QLineEdit(this);
  m_edit->setReadOnly(true);
  m_edit->setMaxLength(20);
  m_edit->setMaximumWidth(mWidth);
  m_edit->setStyleSheet("QLineEdit {border:none;background:transparent;}");
  addWidget(m_edit);
  addSeparator();

  m_buyer = new QLabel(this);
  m_buyer->setObjectName("person");
  m_buyer->setToolTip(tr("Purchaser"));
  m_buyer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  addWidget(m_buyer);

  m_btn = new QPushButton(tr("Check item inventory"), this);
  m_btn->setIcon(QIcon("://icons/action_search.png"));
  QString info = tr("Create a search query to see if all items are available.");
  m_btn->setToolTip(info);
  addWidget(m_btn);

  connect(m_btn, SIGNAL(clicked()), SIGNAL(sendCheckArticles()));
}

void ProviderPurchaseActionBar::setCustomer(const QString &buyer, qint64 cid) {
  if (cid > 0)
    m_edit->setText(QString::number(cid));

  m_buyer->setText(buyer);
}
