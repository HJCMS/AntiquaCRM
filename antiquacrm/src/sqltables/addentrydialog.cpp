// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "addentrydialog.h"
#include "strlineedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

AddEntryDialog::AddEntryDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("AddEntryDialog");

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName("compliterlayout");

  QLabel *info = new QLabel(this);
  info->setText(tr("A new entry require a valid Keyword"));
  verticalLayout->addWidget(info);

  m_lineEdit = new StrLineEdit(this);
  m_lineEdit->setMaxAllowedLength(80);
  verticalLayout->addWidget(m_lineEdit);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  m_buttonBox->setStandardButtons(QDialogButtonBox::Ok |
                                  QDialogButtonBox::Cancel);
  verticalLayout->addWidget(m_buttonBox);

  setLayout(verticalLayout);

  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

const QVariant AddEntryDialog::value() { return m_lineEdit->value(); }
