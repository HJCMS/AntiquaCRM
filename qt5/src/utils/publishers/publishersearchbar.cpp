// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersearchbar.h"

#include <QIcon>
#include <QLabel>

PublisherSearchbar::PublisherSearchbar(QWidget *parent) : QToolBar{parent} {
  setObjectName("publisher_search");
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  m_searchLine = new QLineEdit(this);
  m_searchLine->setClearButtonEnabled(true);
  m_searchLine->setPlaceholderText(tr("Search publisher in table"));
  m_searchLine->setDragEnabled(false);
  m_searchLine->setMaxLength(128);
  addWidget(m_searchLine);

  connect(m_searchLine, SIGNAL(textChanged(const QString &)),
          SLOT(prepareSearch(const QString &)));
  connect(m_searchLine, SIGNAL(returnPressed()), SLOT(prepareSearch()));
}

void PublisherSearchbar::prepareSearch(const QString &search) {
  Q_UNUSED(search);
  QString _str = m_searchLine->text().trimmed();
  if (_str.length() < 3)
    return;

  emit searchPublisher(_str);
}
