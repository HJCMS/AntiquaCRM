// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutdialogs.h"
#include "aboutantiquacrm.h"
#include "aboutpostgresql.h"
#include "aboutqrencode.h"

#include <QLayout>
#include <QPushButton>

AboutDialogs::AboutDialogs(QWidget *parent) : QDialog{parent} {
  setMinimumSize(600, 450);
  setSizeGripEnabled(true);

  Q_INIT_RESOURCE(licences);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_pages = new QStackedWidget(this);

  m_page0 = new AboutAntiquaCRM(this);
  m_pages->insertWidget(0, m_page0);

  m_page1 = new AboutQREncode(this);
  m_pages->insertWidget(1, m_page1);

  m_page2 = new AboutPostgreSQL(this);
  m_pages->insertWidget(2, m_page2);

  layout->addWidget(m_pages);

  QPushButton *btn_ok = new QPushButton(tr("Ok"), this);
  btn_ok->setIcon(QIcon("://icons/action_ok.png"));
  layout->addWidget(btn_ok);

  setLayout(layout);

  connect(btn_ok, SIGNAL(clicked()), SLOT(accept()));
}

int AboutDialogs::exec() { return exec(AboutDialogs::ANTIQUACRM); }

int AboutDialogs::exec(AboutDialogs::Page page) {
  switch (page) {
  case QRENCODE:
    m_pages->setCurrentIndex(1);
    break;

  case POSTGRESQL:
    m_pages->setCurrentIndex(2);
    break;

  default:
    m_pages->setCurrentIndex(0);
  }

  return QDialog::exec();
}

AboutDialogs::~AboutDialogs() { Q_CLEANUP_RESOURCE(licences); }
