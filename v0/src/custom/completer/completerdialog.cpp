// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "completerdialog.h"
#include "completerview.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDialogButtonBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

CompleterDialog::CompleterDialog(QWidget *parent, CompleterDialog::Filter filter)
    : QDialog{parent} {
  setObjectName("CompleterDialogDialog");
  setSizeGripEnabled(true);
  setMinimumSize(QSize(650, 400));

  QString field;
  if (filter == Filter::CONDITION)
  {
    setWindowTitle(tr("Modify Edition dataset"));
    field = "condition";
  } else {
    setWindowTitle(tr("Modify Designation dateset"));
    field = "designation";
  }

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName("compliterlayout");

  QLabel *info = new QLabel(this);
  info->setText(tr("Please enter the keywords here that should be suggested in "
                   "the editor."));
  info->setWordWrap(true);
  verticalLayout->addWidget(info);

  m_tableView = new CompleterView(field,this);
  verticalLayout->addWidget(m_tableView);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  verticalLayout->addWidget(m_buttonBox);

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setObjectName("CloseEditTableDialog");
  m_close->setIcon(myIcon("exit"));
  m_close->setShortcut(QKeySequence::Close);

  setLayout(verticalLayout);

  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
