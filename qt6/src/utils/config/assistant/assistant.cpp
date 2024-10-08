// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "assistant.h"
#include "../configdatabase.h"
#include "assistantwelcome.h"

#include <QDebug>
#include <QMessageBox>
#include <QStyle>
#include <QVBoxLayout>

Assistant::Assistant(QWidget* parent) : QDialog(parent) {
  setObjectName("assistant_dialog");
  setWindowTitle(tr("AntiquaCRM Wizard") + "[*]");
  setWindowIcon(AntiquaCRM::antiquaIcon("antiquacrm"));
  setSizeGripEnabled(true);
  setMinimumSize(700, 500);

  QVBoxLayout* layout = new QVBoxLayout(this);

  m_stackedWidget = new QStackedWidget(this);

  AssistantWelcome* m_page0 = new AssistantWelcome(m_stackedWidget);
  m_page0->setContent("database");
  m_stackedWidget->insertWidget(0, m_page0);

  m_page1 = new ConfigDatabase(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_page1);

  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  m_buttonBox = new QDialogButtonBox(this);
  setButtonBox();
  layout->addWidget(m_buttonBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  // Signals
  connect(m_stackedWidget, SIGNAL(currentChanged(int)), SLOT(pageChanged(int)));
  connect(this, SIGNAL(accepted()), SLOT(beforeAccept()));
  connect(this, SIGNAL(rejected()), SLOT(beforeClose()));
}

void Assistant::setButtonBox() {
  m_buttonBox->setOrientation(Qt::Horizontal);
  QPushButton* btn_restore = m_buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
  btn_restore->setText(tr("Restart"));
  btn_restore->setStatusTip(tr("Restart Assistant"));
  btn_restore->setObjectName("restore_button");
  btn_restore->setIcon(AntiquaCRM::antiquaIcon("action-reload"));
  connect(btn_restore, SIGNAL(clicked()), SLOT(restart()));

  QPushButton* btn_prev = m_buttonBox->addButton(tr("Back"), QDialogButtonBox::ActionRole);
  btn_prev->setStatusTip(tr("Previous page"));
  btn_prev->setObjectName("previous_page_button");
  btn_prev->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
  connect(btn_prev, SIGNAL(clicked()), SLOT(previousPage()));

  QPushButton* btn_next = m_buttonBox->addButton(tr("Next"), QDialogButtonBox::ActionRole);
  btn_next->setStatusTip(tr("Next page"));
  btn_next->setObjectName("next_page_button");
  btn_next->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
  connect(btn_next, SIGNAL(clicked()), SLOT(nextPage()));

  QPushButton* btn_accept = m_buttonBox->addButton(QDialogButtonBox::Save);
  btn_accept->setStatusTip(tr("Save current changes"));
  btn_accept->setObjectName("accept_button");
  connect(btn_accept, SIGNAL(clicked()), SLOT(save()));

  btn_close = m_buttonBox->addButton(QDialogButtonBox::Apply);
  btn_close->setText(tr("Finalize"));
  btn_close->setStatusTip(tr("Finishing and close the assistant"));
  btn_close->setObjectName("close_button");
  btn_close->setEnabled(false);
  connect(btn_close, SIGNAL(clicked()), SLOT(accept()));
}

int Assistant::getPage() {
  return m_stackedWidget->currentIndex();
}

void Assistant::restart() {
  if (getPage() > 0)
    setPage(0);

  unsaved = false;
}

void Assistant::setPage(int index) {
  m_stackedWidget->setCurrentIndex(index);
}

bool Assistant::event(QEvent* e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent* t = static_cast<QStatusTipEvent*>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void Assistant::keyPressEvent(QKeyEvent* e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    // Key press enter or return will ignored!
    return;
  }
  QDialog::keyPressEvent(e);
}

void Assistant::closeEvent(QCloseEvent* e) {
  if (e->type() == QEvent::Close) {
    if (unsaved) {
      int retval = QMessageBox::warning(this, tr("Unfinished Assistant"),
                                        tr("Do not close the wizard without saving if you make "
                                           "changes that you do not want to lose!"),
                                        (QMessageBox::Ok | QMessageBox::Ignore));
      if (retval == QMessageBox::Ok) {
        e->ignore();
        return;
      }
    }
  }
  QDialog::closeEvent(e);
}

void Assistant::previousPage() {
  int _i = getPage();
  if (_i > 0)
    setPage((_i - 1));

  unsaved = true;
}

void Assistant::nextPage() {
  int _i = getPage();
  if (_i >= 0 && _i < m_stackedWidget->count())
    setPage((_i + 1));

  unsaved = true;
}

void Assistant::beforeAccept() {
  qInfo("Assistant finished!");
}

void Assistant::beforeClose() {
  unsaved = true;
  qInfo("Assistant canceled!");
}

void Assistant::pageChanged(int index) {
  if (index == 1) {
    m_page1->loadSectionConfig();
  }
}

void Assistant::save() {
  if (getPage() == 1) {
    m_page1->saveSectionConfig();
    m_statusBar->showMessage(tr("Success"));
    unsaved = false;
    btn_close->setEnabled(true);
    return;
  }
  unsaved = true;
}

Assistant::~Assistant() {
}
