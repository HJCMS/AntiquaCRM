// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersedit.h"
#include "publishersearchbar.h"
#include "publishersinput.h"
#include "publishersview.h"

#include <QDebug>
#include <QMap>
#include <QVBoxLayout>

PublishersEdit::PublishersEdit(QWidget *parent) : QDialog{parent} {
  setObjectName("keyword_edit_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(500, 500);
  setContentsMargins(2, 2, 2, 2);

  m_sql = new AntiquaCRM::ASqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_searchBar = new PublisherSearchbar(this);
  layout->addWidget(m_searchBar);

  m_view = new PublishersView(this);
  layout->addWidget(m_view);
  layout->setStretch(1, 1);

  m_input = new PublishersInput(this);
  layout->addWidget(m_input);

  m_buttonsBar = new QDialogButtonBox(Qt::Horizontal, this);
  btn_save = m_buttonsBar->addButton(QDialogButtonBox::Save);
  btn_close = m_buttonsBar->addButton(QDialogButtonBox::Close);
#ifdef Q_OS_WIN
  btn_save->setIcon(QIcon(":/icons/action_save.png"));
  btn_close->setIcon(QIcon(":/icons/action_quit.png"));
#endif
  layout->addWidget(m_buttonsBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);
  connect(btn_save, SIGNAL(clicked()), SLOT(prepareAction()));
  connect(btn_close, SIGNAL(clicked()), SLOT(reject()));

  connect(m_view, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(rowSelected(const QModelIndex &)));

  connect(m_searchBar, SIGNAL(searchPublisher(const QString &)), m_view,
          SLOT(showPublisher(const QString &)));
}

void PublishersEdit::deleteEntry() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void PublishersEdit::updateEntry() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void PublishersEdit::insertEntry() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void PublishersEdit::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    // Key press enter or return will ignored!
    return;
  }
  QDialog::keyPressEvent(e);
}

bool PublishersEdit::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = static_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

void PublishersEdit::rowSelected(const QModelIndex &index) {
  QPair<QString, QString> _p = m_view->getData(index);
  m_input->setPublisher(_p.first);
  m_input->setLocation(_p.second);
}

void PublishersEdit::prepareAction() {
  QString _p = m_input->getPublisher();
  QString _l = m_input->getLocation();
  if (_p.length() < 2 || _l.length() < 2) {
    m_statusBar->showMessage(tr("Missing values for action."), (10 * 1000));
    return;
  }

  if (m_view->find(_p).isValid())
    updateEntry();
  else
    insertEntry();
}

int PublishersEdit::exec() {
  if (m_view->init()) {
    qDebug() << Q_FUNC_INFO << "__TODO__";
  }
  return QDialog::exec();
}
