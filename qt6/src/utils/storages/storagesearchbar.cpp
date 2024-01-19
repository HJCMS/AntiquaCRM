// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagesearchbar.h"

#include <QLabel>
#include <QLayout>

StorageSearchBar::StorageSearchBar(QWidget *parent) : QWidget{parent} {
  setContentsMargins(1, 1, 1, 1);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(contentsMargins());

  m_searchLine = new AntiquaCRM::ALineEdit(this);
  m_searchLine->setMinLength(3);
  m_searchLine->setMinimumWidth(150);
  layout->addWidget(m_searchLine);
  layout->setStretch(0, 1);

  btn_search = new QPushButton(tr("Search"), this);
  btn_search->setIcon(AntiquaCRM::antiquaIcon("action-search"));
  btn_search->setStatusTip(tr("Search storage by keyword or name."));
  layout->addWidget(btn_search);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_search, SIGNAL(clicked()), SLOT(searchClicked()));
  connect(m_searchLine, SIGNAL(editingFinished()), SLOT(searchClicked()));
  connect(m_searchLine, SIGNAL(returnPressed()), SLOT(searchClicked()));
  connect(m_searchLine, SIGNAL(textChanged(const QString &)),
          SLOT(onSearchLineEdit(const QString &)));
}

void StorageSearchBar::onSearchLineEdit(const QString &search) {
  if (search.length() > 1)
    emit sendFindStorage(search);
}

void StorageSearchBar::searchClicked() {
  onSearchLineEdit(m_searchLine->text().trimmed());
}
