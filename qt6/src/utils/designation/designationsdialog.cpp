// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationsdialog.h"
#include "designationedit.h"
#include "designationslist.h"

#include <AntiquaInput>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QStatusTipEvent>
#include <QVBoxLayout>

DesignationsDialog::DesignationsDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("designations_edit_dialog");
  setSizeGripEnabled(true);
  setWindowTitle(tr("Edit Designation") + "[*]");
  setMinimumSize(500, 450);

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
  btn_delete = new QPushButton(AntiquaCRM::antiquaIcon("action-remove"),
                               tr("Delete"), m_statusBar);
  btn_delete->setToolTip(tr("Delete current Entry"));
  btn_delete->setStatusTip(btn_delete->toolTip());
  m_statusBar->addPermanentWidget(btn_delete);
  // Neuer Eintrag
  btn_clear = new QPushButton(AntiquaCRM::antiquaIcon("edit-clear"),
                              tr("Clear"), m_statusBar);
  btn_clear->setToolTip(tr("Clear input to create a new Entry"));
  btn_clear->setStatusTip(btn_clear->toolTip());
  m_statusBar->addPermanentWidget(btn_clear);
  // Speichern
  btn_save = new QPushButton(AntiquaCRM::antiquaIcon("action-save"), tr("Save"),
                             m_statusBar);
  btn_save->setToolTip(tr("Save current Dataset."));
  m_statusBar->addPermanentWidget(btn_save);
  // Schliessen
  btn_close = new QPushButton(AntiquaCRM::antiquaIcon("action-quit"),
                              tr("Close"), m_statusBar);
  btn_close->setToolTip(tr("Exiting this Dialog."));
  m_statusBar->addPermanentWidget(btn_close);

  layout->addWidget(m_statusBar);
  layout->setStretch(0, 2);
  layout->setStretch(2, 1);

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
  if (m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("Saving success!"));
    loadBookDesignations();
    m_edit->setClear();
  } else {
    m_statusBar->showMessage(tr("An error has occurred!"));
  }
}

int DesignationsDialog::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!loadBookDesignations())
    return QDialog::Rejected;

  return QDialog::exec();
}

DesignationsDialog::~DesignationsDialog() {
  if (m_sql != nullptr)
    m_sql->deleteLater();
}
