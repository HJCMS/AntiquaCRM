// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "addentrydialog.h"
#include "strlineedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtWidgets/QAction>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

AddEntryDialog::AddEntryDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("AddEntryDialog");
  setWindowTitle(tr("Add new entry"));
  setWindowIcon(myIcon("database"));

  QVBoxLayout *verticalLayout = new QVBoxLayout(this);
  verticalLayout->setObjectName("compliterlayout");

  QLabel *info = new QLabel(this);
  info->setText(tr("A new entry require a valid Keyword"));
  verticalLayout->addWidget(info);

  m_keyword = new StrLineEdit(this);
  m_keyword->setMaxAllowedLength(80);
  verticalLayout->addWidget(m_keyword);

  QLabel *desc = new QLabel(this);
  desc->setText(tr("Description"));
  verticalLayout->addWidget(desc);

  m_description = new QTextEdit(this);
  m_description->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
  verticalLayout->addWidget(m_description);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  m_buttonBox->setStandardButtons(QDialogButtonBox::Ok |
                                  QDialogButtonBox::Cancel);
  verticalLayout->addWidget(m_buttonBox);

  setLayout(verticalLayout);

  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void AddEntryDialog::setKeyword(const QString &s) { m_keyword->setValue(s); }

void AddEntryDialog::setDescription(const QString &s) {
  m_description->setPlainText(s);
}

const QString AddEntryDialog::keyword() {
  QRegExp reg("[\\'\\n\\r\\t]+");
  QString txt = m_keyword->value().toString();
  return txt.replace(reg, "");
}

const QString AddEntryDialog::description() {
  QRegExp reg("[\\'\\n\\r\\t]+");
  QString txt = m_description->toPlainText();
  return txt.replace(reg, "");
}
