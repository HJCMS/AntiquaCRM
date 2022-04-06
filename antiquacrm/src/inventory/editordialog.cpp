// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editordialog.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QDialogButtonBox>

EditorDialog::EditorDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("EditorDialog");
  setSizeGripEnabled(true);
  setMinimumSize(800,600);

  m_layout = new QVBoxLayout(this);
  m_layout->setObjectName("EditorDialogLayout");
  QDialogButtonBox::StandardButtons btnset(QDialogButtonBox::Save |
                                           QDialogButtonBox::Close |
                                           QDialogButtonBox::RestoreDefaults);

  m_buttonBox = new QDialogButtonBox(btnset, Qt::Horizontal, this);
  m_layout->insertWidget(1, m_buttonBox);

  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(saveClicked()));
  connect(m_buttonBox, SIGNAL(clicked(QAbstractButton *)), this,
          SLOT(buttonClicked(QAbstractButton *)));
}

void EditorDialog::saveClicked() { emit s_commitDataset(); }

void EditorDialog::buttonClicked(QAbstractButton *r) {
  if (m_buttonBox->buttonRole(r) == QDialogButtonBox::ResetRole) {
    emit s_restoreDataset();
  }
}

void EditorDialog::setMainWidget(QWidget *w) { m_layout->insertWidget(0, w); }
