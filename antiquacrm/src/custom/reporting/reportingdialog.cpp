// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportingdialog.h"
#include "myicontheme.h"

#include <QDebug>
#include <QLayout>
#include <QStatusTipEvent>

ReportingDialog::ReportingDialog(QWidget *parent) : QDialog{parent} {
  setMinimumSize(500, 500);
  setSizeGripEnabled(true);
  setContentsMargins(5, 5, 5, 5);
  setWindowTitle("AntiquaCRM " + tr("Reports") + " [*]");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_infoLabel = new QLabel(this);
  m_infoLabel->setText(tr("Export various reports."));
  layout->addWidget(m_infoLabel);

  m_stackedWidget = new QStackedWidget(this);
  layout->addWidget(m_stackedWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setOrientation(Qt::Horizontal);
  btn_prev = m_btnBox->addButton(tr("Back"), QDialogButtonBox::ActionRole);
  btn_prev->setObjectName("btn_previous");
  btn_prev->setIcon(myIcon("edit_remove"));
  btn_prev->setEnabled(false);

  btn_next = m_btnBox->addButton(tr("Next"), QDialogButtonBox::ActionRole);
  btn_next->setObjectName("btn_next");
  btn_next->setIcon(myIcon("edit_add"));

  btn_apply = m_btnBox->addButton(tr("Apply"), QDialogButtonBox::AcceptRole);
  btn_apply->setObjectName("btn_apply");
  btn_apply->setIcon(myIcon("button_ok"));
  btn_apply->setEnabled(false);

  btn_quit = m_btnBox->addButton(tr("Quit"), QDialogButtonBox::RejectRole);
  btn_quit->setObjectName("btn_quit");
  btn_quit->setIcon(myIcon("button_cancel"));
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(btn_prev, SIGNAL(clicked()), this, SLOT(previousPage()));
  connect(btn_next, SIGNAL(clicked()), this, SLOT(nextPage()));
  connect(btn_apply, SIGNAL(clicked()), this, SLOT(apply()));
  connect(btn_quit, SIGNAL(clicked()), this, SLOT(close()));
}

int ReportingDialog::pageIndex() { return m_stackedWidget->currentIndex(); }

void ReportingDialog::setPage(int index) {
  m_stackedWidget->setCurrentIndex(index);
}

void ReportingDialog::previousPage() {
  if (pageIndex() > 0)
    setPage((pageIndex() - 1));
}

void ReportingDialog::nextPage() {
  if (pageIndex() >= 0 && pageIndex() < m_stackedWidget->count())
    setPage((pageIndex() + 1));
}

void ReportingDialog::pageEntered(int index) {
  int c = m_stackedWidget->count();
  btn_prev->setEnabled((index == 0) ? false :true);
  btn_next->setEnabled((index == c) ? false :true);
  btn_apply->setEnabled((index == c));
}

void ReportingDialog::close() {
  qDebug() << Q_FUNC_INFO << "TODO";
  reject();
}

void ReportingDialog::apply() {
  qDebug() << Q_FUNC_INFO << "TODO";
  // accept();
}

void ReportingDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    return;
  }
  QDialog::keyPressEvent(e);
}

bool ReportingDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

int ReportingDialog::exec() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return QDialog::exec();
}
