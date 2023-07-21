// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutdialogs.h"
#include "aboutantiquacrm.h"
#include "aboutpostgresql.h"
#include "aboutqrencode.h"

#include <AntiquaInput>
#include <QApplication>
#include <QLayout>
#include <QPushButton>

AboutDialogs::AboutDialogs(QWidget *parent) : QDialog{parent} {
  setMinimumSize(600, 450);
  setSizeGripEnabled(true);

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
  btn_ok->setIcon(AntiquaCRM::antiquaIcon("action-ok"));
  layout->addWidget(btn_ok);

  setLayout(layout);

  connect(btn_ok, SIGNAL(clicked()), SLOT(accept()));
}

int AboutDialogs::exec() { return exec(AboutDialogs::ANTIQUACRM); }

int AboutDialogs::exec(AboutDialogs::Page page) {
  switch (page) {
  case AboutDialogs::Page::ANTIQUACRM:
    m_pages->setCurrentIndex(0);
    break;

  case AboutDialogs::Page::QRENCODE:
    m_pages->setCurrentIndex(1);
    break;

  case AboutDialogs::Page::POSTGRESQL:
    m_pages->setCurrentIndex(2);
    break;

  default:
    m_pages->setCurrentIndex(0);
  }

  return QDialog::exec();
}

AboutMenu::AboutMenu(QWidget *parent) : QMenu{tr("About"), parent} {
  setObjectName("antiquacrm_about_menu");

  Q_INIT_RESOURCE(licences);

  m_mapper = new QSignalMapper(this);

  QAction *ac_about_crm = addAction("Antiqua CRM");
  ac_about_crm->setIcon(AntiquaCRM::antiquaIcon("antiquacrm"));
  m_mapper->setMapping(ac_about_crm, AboutDialogs::Page::ANTIQUACRM);
  connect(ac_about_crm, SIGNAL(triggered()), m_mapper, SLOT(map()));

  // https://www.qt.io
  QAction *ac_about_qt = addAction("Qt Framework");
  ac_about_qt->setIcon(QIcon(":/qtlogo-64.png"));
  connect(ac_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QAction *ac_about_qr = addAction("QRencode");
  ac_about_qr->setIcon(AntiquaCRM::antiquaIcon("help-about"));
  m_mapper->setMapping(ac_about_qr, AboutDialogs::Page::QRENCODE);
  connect(ac_about_qr, SIGNAL(triggered()), m_mapper, SLOT(map()));

  QAction *ac_about_sql = addAction("PostgreSQL");
  ac_about_sql->setIcon(QIcon(":/postgresql.png"));
  m_mapper->setMapping(ac_about_sql, AboutDialogs::Page::POSTGRESQL);
  connect(ac_about_sql, SIGNAL(triggered()), m_mapper, SLOT(map()));

  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(openDialog(int)));
}

void AboutMenu::openDialog(int page) {
  AboutDialogs::Page _page = static_cast<AboutDialogs::Page>(page);
  AboutDialogs *d = new AboutDialogs(parentWidget());
  d->exec(_page);
  d->deleteLater();
}

AboutMenu::~AboutMenu() { Q_CLEANUP_RESOURCE(licences); }
