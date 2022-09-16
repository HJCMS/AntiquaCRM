// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstoolbar.h"
#include "myicontheme.h"
#include "ordersmenubutton.h"

#include <QDebug>
#include <QLayout>
#include <QSpacerItem>

OrdersToolBar::OrdersToolBar(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(4, 0, 4, 0);

  m_infoBtn = new QToolButton(this);
  m_infoBtn->setEnabled(false);
  m_infoBtn->setIcon(myIcon("search"));
  m_infoBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
  layout->addWidget(m_infoBtn);

  m_searchSection = new QComboBox(this);
  m_searchSection->insertItem(0, myIcon("autostart"), tr("Order Id"),
                              SEARCH_ORDER_ID);
  m_searchSection->insertItem(1, myIcon("group"), tr("Customer name"),
                              SEARCH_CUSTOMER);
  m_searchSection->insertItem(2, myIcon("autostart"),
                              tr("Provider settlement number"),
                              SEARCH_PROVIDER);
  layout->addWidget(m_searchSection);

  m_searchLineEdit = new QLineEdit(this);
  QString info = tr("Search for %1 in current View.").arg(tr("Order Id"));
  m_searchLineEdit->setMinimumWidth(250);
  m_searchLineEdit->setPlaceholderText(info);
  layout->addWidget(m_searchLineEdit);

  QSpacerItem *sp = new QSpacerItem(5, 5, QSizePolicy::MinimumExpanding,
                                    QSizePolicy::Preferred);
  layout->addSpacerItem(sp);

  m_menuButton = new OrdersMenuButton(this);
  layout->addWidget(m_menuButton);

  btn_refresh = new QPushButton(myIcon("reload"), tr("Refresh"), this);
  layout->addWidget(btn_refresh);

  setLayout(layout);

  connect(m_searchSection, SIGNAL(currentIndexChanged(int)), this,
          SLOT(searchSectionChanged(int)));
  connect(m_searchLineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(searchTextChanged(const QString &)));
  connect(m_menuButton, SIGNAL(sendDefaultView()), this,
          SIGNAL(sendDefaultView()));
  connect(m_menuButton, SIGNAL(sendCustomQuery(const QString &)), this,
          SIGNAL(sendCustomQuery(const QString &)));
  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendRefreshView()));

  p_searchText = QString();
  setSearchSection(OrdersToolBar::SEARCH_CUSTOMER);
}

void OrdersToolBar::searchSectionChanged(int index) {
  QString title = m_searchSection->itemData(index, Qt::DisplayRole).toString();
  QString info = tr("Search for %1 in current View.").arg(title);
  m_searchLineEdit->setPlaceholderText(info);

  int val = m_searchSection->itemData(index, Qt::UserRole).toInt();
  section = static_cast<OrdersToolBar::Search>(val);
  emit sendSectionChanged(section);
}

void OrdersToolBar::searchTextChanged(const QString &str) {
  QString search = str.trimmed();
  if (search.length() > 1) {
    p_searchText = search;
    emit sendSearchText(search);
  }
}

OrdersToolBar::Search OrdersToolBar::getSearchSection() { return section; }

const QString OrdersToolBar::getSearchLineEdit() {
  QString buffer = m_searchLineEdit->text().trimmed();
  return buffer;
}

void OrdersToolBar::setSearchSection(OrdersToolBar::Search sec) {
  for (int i = 0; i < m_searchSection->count(); i++) {
    int val = m_searchSection->itemData(i, Qt::UserRole).toInt();
    if (static_cast<OrdersToolBar::Search>(val) == sec) {
      m_searchSection->setCurrentIndex(i);
    }
  }
}
