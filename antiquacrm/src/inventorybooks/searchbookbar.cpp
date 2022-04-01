// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchbookbar.h"
#include "combofieldbox.h"
#include "searchlineedit.h"
#include "version.h"
#include "pmetatypes.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

SearchBookBar::SearchBookBar(QWidget *parent)
    : QToolBar(parent), minlength(10) {
  setObjectName("SearchBookBar");
  setOrientation(Qt::Horizontal);
  setAllowedAreas((Qt::TopToolBarArea | Qt::BottomToolBarArea));

  QToolButton *m_tb = new QToolButton(this);
  m_tb->setEnabled(false);
  m_tb->setIcon(myIcon("search"));
  addWidget(m_tb);
  addSeparator();

  m_input = new SearchLineEdit(this);
  addWidget(m_input);
  addSeparator();

  QPushButton *clearBtn = new QPushButton(this);
  clearBtn->setIcon(myIcon("clear_left"));
  clearBtn->setToolTip(tr("Clear searchinput"));
  addWidget(clearBtn);
  addSeparator();

  QPushButton *searchBtn = new QPushButton(tr("Search"), this);
  searchBtn->setObjectName("SearchButton");
  searchBtn->setToolTip(tr("Start search"));
  searchBtn->setContentsMargins(5, 0, 15, 0);
  addWidget(searchBtn);
  addSeparator();

  m_comboBox = new ComboFieldBox(this);
  addWidget(m_comboBox);
  addSeparator();

  // SIGNALS
  connect(m_input, SIGNAL (inputChanged(const QString &)),
          this, SLOT (lineEditChanged(const QString &)));

  connect(clearBtn, SIGNAL (pressed()), m_input, SLOT(clear()));
  connect(searchBtn, SIGNAL (released()), this, SLOT(startSearchClicked()));
}

void SearchBookBar::lineEditChanged(const QString &str) {
  if (str.length() <= minlength)
    return;

  startSearchClicked();
}

void SearchBookBar::startSearchClicked() {
  if (m_input->text().length() < 2)
    return;

  QString buf = m_input->text();
  QRegExp reg("[\\'\\\"]+");
  buf.replace(reg, "");

  reg.setPattern("(\\s|\\t)+");
  buf.replace(reg, " ");

  buf = buf.trimmed();

  if (buf.length() >= 2) {
    SearchStatement s;
    s.SearchField = m_comboBox->currentData(Qt::UserRole).toString();
    s.SearchString = buf;
    qDebug("QueryStatement:'%s':'%s'", qPrintable(s.SearchField),
           qPrintable(s.SearchString));
    emit s_sqlQueryChanged(s);
  }
}
