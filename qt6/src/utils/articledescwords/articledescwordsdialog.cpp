// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articledescwordsdialog.h"
#include "articledescwordswidget.h"

#include <AntiquaWidgets>
#include <QIcon>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>

ArticleDescWordsDialog::ArticleDescWordsDialog(QWidget* parent) : QDialog{parent} {
  setWindowTitle(tr("Description Settings") + " [*]");
  setSizeGripEnabled(true);
  setMinimumSize(780, 540);

  QVBoxLayout* layout = new QVBoxLayout(this);

  QScrollArea* m_scrollWidget = new QScrollArea(this);
  m_scrollWidget->setWidgetResizable(true);
  layout->addWidget(m_scrollWidget);

  m_editWidget = new ArticleDescWordsWidget(m_scrollWidget);
  m_scrollWidget->setWidget(m_editWidget);

  m_btnBox = new QDialogButtonBox(this);
  m_btnBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Close);
  layout->addWidget(m_btnBox);

  QPushButton* btn_save = m_btnBox->button(QDialogButtonBox::Ok);
  btn_save->setIcon(AntiquaCRM::antiquaIcon("action-save"));
  btn_save->setText(tr("Complete"));
  btn_save->setToolTip(tr("Save data and close this dialog."));

  QPushButton* btn_close = m_btnBox->button(QDialogButtonBox::Close);
  btn_close->setIcon(AntiquaCRM::antiquaIcon("action-quit"));
  btn_close->setText(tr("Cancel"));
  btn_close->setToolTip(tr("Close dialog without saving."));

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_editWidget, SIGNAL(sendCloseDialog(bool)), SLOT(closeDialog(bool)));
  connect(btn_save, SIGNAL(clicked()), m_editWidget, SLOT(sqlCommit()));
  connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
}

void ArticleDescWordsDialog::keyPressEvent(QKeyEvent* e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    return;
  }
  QDialog::keyPressEvent(e);
}

bool ArticleDescWordsDialog::event(QEvent* e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent* t = reinterpret_cast<QStatusTipEvent*>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void ArticleDescWordsDialog::closeDialog(bool b) {
  if (b) {
    m_statusBar->showMessage(tr("Description data saving success!"));
    QTimer::singleShot(1000, this, SLOT(accept()));
    return;
  }
  m_statusBar->showMessage(tr("An error has occurred!"));
}

int ArticleDescWordsDialog::exec() {
  if (!m_editWidget->sqlQuery()) {
    qWarning("Descriptions Dialog: SQL Connection error!");
    return QDialog::Rejected;
  }
  return QDialog::exec();
}
