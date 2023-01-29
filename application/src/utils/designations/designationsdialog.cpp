// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationsdialog.h"
#include "designationedit.h"
#include "designationslist.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QStatusTipEvent>
#include <QVBoxLayout>

DesignationsDialog::DesignationsDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("designations_edit_dialog");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Edit Designation") + "[*]");
  setMinimumSize(600, 450);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_listView = new DesignationsList(this);
  layout->addWidget(m_listView);

  QLabel *m_label = new QLabel(this);
  m_label->setText(tr("Edit predefined book cover description."));
  layout->addWidget(m_label);

  m_edit = new DesignationEdit(this);
  layout->addWidget(m_edit);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  // Eintrag löschen
  btn_delete = new QPushButton(QIcon("://icons/action_remove.png"),
                               tr("Delete"), m_statusBar);
  btn_delete->setToolTip(tr("Delete current Entry"));
  btn_delete->setStatusTip(btn_delete->toolTip());
  m_statusBar->addPermanentWidget(btn_delete);
  // Neuer Eintrag
  btn_clear = new QPushButton(QIcon("://icons/editclear.png"), tr("Clear"),
                              m_statusBar);
  btn_clear->setToolTip(tr("Clear input to create a new Entry"));
  btn_clear->setStatusTip(btn_clear->toolTip());
  m_statusBar->addPermanentWidget(btn_clear);
  // Speichern
  btn_save = new QPushButton(QIcon("://icons/action_save.png"), tr("Save"),
                             m_statusBar);
  btn_save->setToolTip(tr("Save current Dataset."));
  m_statusBar->addPermanentWidget(btn_save);
  // Schliessen
  btn_close = new QPushButton(QIcon("://icons/action_quit.png"), tr("Close"),
                              m_statusBar);
  btn_close->setToolTip(tr("Exiting this Dialog."));
  m_statusBar->addPermanentWidget(btn_close);

  layout->addWidget(m_statusBar);

  setLayout(layout);

  connect(m_listView, SIGNAL(sendEditItem(const DesignationData &)), m_edit,
          SLOT(setEditItem(const DesignationData &)));

  connect(m_edit, SIGNAL(sendSaveQuery(const QString &)),
          SLOT(setSaveQuery(const QString &)));

  connect(m_edit, SIGNAL(sendStatusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(btn_clear, SIGNAL(clicked()), m_edit, SLOT(setCreateNew()));
  connect(btn_delete, SIGNAL(clicked()), m_edit, SLOT(setDelete()));
  connect(btn_save, SIGNAL(clicked()), m_edit, SLOT(setSave()));
  connect(btn_close, SIGNAL(clicked()), SLOT(reject()));
}

void DesignationsDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
    return;
  }
  QDialog::keyPressEvent(e);
}

void DesignationsDialog::resizeEvent(QResizeEvent *e) {
  if (e->type() == QEvent::Resize) {
    m_cfg->beginGroup("dialog/designation");
    m_cfg->setValue("geometry", saveGeometry());
    m_cfg->endGroup();
  }
  QDialog::resizeEvent(e);
}

bool DesignationsDialog::event(QEvent *e) {
  if (e->type() == QEvent::StatusTip) {
    QStatusTipEvent *t = reinterpret_cast<QStatusTipEvent *>(e);
    if (t->tip().isEmpty())
      return false;

    m_statusBar->showMessage(t->tip(), 1000);
    return true;
  }
  return QDialog::event(e);
}

bool DesignationsDialog::loadBookDesignations() {
  AntiquaCRM::ASqlFiles sqlFile("query_designation_edit");
  if (!sqlFile.openTemplate())
    return false;

  QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
  if (q.size() > 0) {
    m_listView->clear();
    m_listView->insertQuery(q);
    return true;
  }
  return false;
}

void DesignationsDialog::setSaveQuery(const QString &sql) {
  m_sql->query(sql);
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << sql  // SQL Query
           << m_sql->lastError(); // Fehler?
#endif
  if (m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("Saving success!"));
    loadBookDesignations();
    m_edit->setClear();
  } else {
    m_statusBar->showMessage(tr("An error has occurred!"));
  }
}

int DesignationsDialog::exec() {
  m_cfg = new AntiquaCRM::ASettings(this);
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!loadBookDesignations())
    return QDialog::Rejected;

  if (m_cfg->contains("dialog/designation/geometry")) {
    m_cfg->beginGroup("dialog/designation");
    restoreGeometry(m_cfg->value("geometry").toByteArray());
    m_cfg->endGroup();
  }
  return QDialog::exec();
}

DesignationsDialog::~DesignationsDialog() {
  if (m_cfg != nullptr)
    m_cfg->deleteLater();

  if (m_sql != nullptr)
    m_sql->deleteLater();
}
