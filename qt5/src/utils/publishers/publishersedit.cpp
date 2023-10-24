// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersedit.h"
#include "publishersearchbar.h"
#include "publishersinput.h"
#include "publishersview.h"

#include <QDebug>
#include <QMap>
#include <QPushButton>
#include <QVBoxLayout>

PublishersEdit::PublishersEdit(QWidget *parent) : QDialog{parent} {
  setObjectName("keyword_edit_dialog");
  setWindowTitle(tr("Publisher Editor") + " [*]");
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
  QPushButton *btn_save = m_buttonsBar->addButton(QDialogButtonBox::Save);
  btn_save->setToolTip(tr("Save current changes"));
  btn_save->setStatusTip(btn_save->toolTip());

  QPushButton *btn_close = m_buttonsBar->addButton(QDialogButtonBox::Close);
  btn_close->setToolTip(tr("Close this editor"));
  btn_close->setStatusTip(btn_close->toolTip());

#ifdef Q_OS_WIN
  btn_save->setIcon(QIcon(":/icons/action_save.png"));
  btn_close->setIcon(QIcon(":/icons/action_quit.png"));
#endif

  QPushButton *btn_new = new QPushButton(tr("new entry"), this);
  btn_new->setIcon(QIcon(":/icons/action_add.png"));
  btn_new->setToolTip(tr("Clear input mask for a new entry."));
  btn_new->setStatusTip(btn_new->toolTip());
  m_buttonsBar->addButton(btn_new, QDialogButtonBox::ActionRole);

  layout->addWidget(m_buttonsBar);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  setLayout(layout);
  // SearchBar
  connect(m_searchBar, SIGNAL(searchPublisher(const QString &)), m_view,
          SLOT(showPublisher(const QString &)));
  // TableView
  connect(m_view, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(rowSelected(const QModelIndex &)));
  // ButtonBox
  connect(btn_new, SIGNAL(clicked()), m_input, SLOT(clear()));
  connect(btn_save, SIGNAL(clicked()), SLOT(prepareAction()));
  connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
}

void PublishersEdit::updateEntry(const QString &publisher) {
  QPair<QString, QString> _p = m_view->getData(m_view->index(publisher));
  QString _sql("UPDATE " + m_view->getTableName() + " SET ");
  _sql.append("p_name='" + m_input->getPublisher() + "', ");
  _sql.append("p_location='" + m_input->getLocation() + "' ");
  _sql.append("WHERE (p_name='" + _p.first + "' AND ");
  _sql.append("p_location='" + _p.second + "');");
  if (m_view->commitQuery(_sql)) {
    m_statusBar->showMessage(tr("Success"));
    m_view->reload();
  }
}

void PublishersEdit::insertEntry() {
  QString _sql("INSERT INTO " + m_view->getTableName());
  _sql.append(" (p_name,p_location) VALUES (");
  _sql.append("'" + m_input->getPublisher() + "',");
  _sql.append("'" + m_input->getLocation() + "');");
  if (m_view->commitQuery(_sql)) {
    m_statusBar->showMessage(tr("Success"));
    m_view->reload();
  }
}

void PublishersEdit::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    // Key press enter or return must ignored!
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

  if (m_view->index(_p).isValid())
    updateEntry(_p);
  else
    insertEntry();
}

int PublishersEdit::exec() {
  if (!m_view->init())
    return QDialog::Rejected;

  return QDialog::exec();
}
