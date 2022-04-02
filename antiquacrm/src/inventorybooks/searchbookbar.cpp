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
  setFloatable(false);

  QToolButton *m_tb = new QToolButton(this);
  m_tb->setEnabled(false);
  m_tb->setIcon(myIcon("search"));
  addWidget(m_tb);
  addSeparator();

  m_comboBox = new ComboFieldBox(this);
  addWidget(m_comboBox);
  addSeparator();

  m_input = new SearchLineEdit(this);
  addWidget(m_input);
  addSeparator();

  QAction *m_clear = addAction(tr("Clear"));
  m_clear->setIcon(myIcon("clear_left"));
  m_clear->setToolTip(tr("Clear searchinput"));

  addSeparator();

  QPushButton *searchBtn = new QPushButton(tr("Start search"), this);
  searchBtn->setObjectName("SearchButton");
  searchBtn->setToolTip(tr("Start search"));
  searchBtn->setIcon(myIcon("search"));
  searchBtn->setContentsMargins(5, 0, 15, 0);
  addWidget(searchBtn);
  addSeparator();

  // SIGNALS
  connect(m_input, SIGNAL (inputChanged(const QString &)),
          this, SLOT (lineEditChanged(const QString &)));

  connect(m_input, SIGNAL(returnPressed()), this, SLOT (startSearchClicked()));

  connect(m_clear, SIGNAL (triggered()), m_input, SLOT(clear()));
  connect(searchBtn, SIGNAL (released()), this, SLOT(startSearchClicked()));

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)),
          m_input,SLOT(updatePlaceHolder(int)));
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
