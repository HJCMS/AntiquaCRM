// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editordialog.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>

EditorDialog::EditorDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("EditorDialog");
  setSizeGripEnabled(true);
  setMinimumSize(800, 600);

  m_layout = new QVBoxLayout(this);
  m_layout->setObjectName("EditorDialogLayout");
  m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);

  m_layout->insertWidget(1, m_buttonBox);

  QPushButton *m_save =
      m_buttonBox->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
  m_save->setObjectName("SaveButtonDialog");
  m_save->setToolTip(tr("Save current dataset"));
  m_save->setIcon(myIcon("filesave"));
  m_save->setShortcut(QKeySequence::Save);

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setObjectName("CloseButtonDialog");
  m_close->setToolTip(tr("Close the dialog without saving."));
  m_close->setIcon(myIcon("exit"));
  m_close->setShortcut(QKeySequence::Close);

  QPushButton *m_restore =
      m_buttonBox->addButton(tr("Restore"), QDialogButtonBox::ResetRole);
  m_restore->setObjectName("RestoreButtonDialog");
  m_restore->setToolTip(tr("Restore dataset to last save operation."));
  m_restore->setIcon(myIcon("rebuild"));
  m_restore->setShortcut(QKeySequence::Undo);

  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(saveClicked()));
  connect(m_buttonBox, SIGNAL(clicked(QAbstractButton *)), this,
          SLOT(buttonClicked(QAbstractButton *)));
}

void EditorDialog::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_Return) {
    qInfo("Press enter key will ignored in editor dialog");
    return;
  }
  QDialog::keyPressEvent(e);
}

void EditorDialog::saveClicked() { emit s_commitDataset(); }

void EditorDialog::buttonClicked(QAbstractButton *r) {
  if (m_buttonBox->buttonRole(r) == QDialogButtonBox::ResetRole) {
    emit s_restoreDataset();
  }
}

void EditorDialog::setMainWidget(QWidget *w) { m_layout->insertWidget(0, w); }
