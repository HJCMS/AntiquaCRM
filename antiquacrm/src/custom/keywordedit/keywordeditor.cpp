// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordeditor.h"
#include "myicontheme.h"

#include <QGridLayout>
#include <QJsonValue>
#include <QLabel>

KeywordEditor::KeywordEditor(QWidget *parent) : QGroupBox{parent} {
  setObjectName("keyword_editor");
  setTitle(tr("Create/Edit Keyword"));

  QGridLayout *layout = new QGridLayout(this);

  QLabel *infoKeyword = new QLabel(this);
  infoKeyword->setText(tr("Keyword") + ":");
  layout->addWidget(infoKeyword, 0, 0, 1, 1, Qt::AlignRight);

  m_keyword = new QLineEdit(this);
  m_keyword->setToolTip(tr("Single Keyword"));
  layout->addWidget(m_keyword, 0, 1, 1, 2);

  m_validator = new QRegExpValidator(p_validateRegExp, m_keyword);
  m_keyword->setValidator(m_validator);

  m_booksUsage = new QCheckBox(tr("Books Keyword"), this);
  m_booksUsage->setToolTip(tr("Use this entry for book keywords"));
  layout->addWidget(m_booksUsage, 1, 0, 1, 1);

  m_id = new QSpinBox(this);
  m_id->setToolTip(tr("Database Id"));
  m_id->setRange(0, 32766);
  m_id->setValue(0);
  m_id->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_id->setReadOnly(true);
  layout->addWidget(m_id, 1, 1, 1, 1, Qt::AlignLeft);

  QHBoxLayout *hLayout = new QHBoxLayout();

  m_btnClear = new QPushButton(tr("Clear"), this);
  m_btnClear->setIcon(myIcon("editclear"));
  m_btnClear->setToolTip(tr("Clear all fields, to create a new Keyword."));
  hLayout->addWidget(m_btnClear);

  hLayout->addStretch(1);

  m_btnCommit = new QPushButton(tr("Save"), this);
  m_btnCommit->setIcon(myIcon("db_comit"));
  hLayout->addWidget(m_btnCommit);

  layout->addLayout(hLayout, 1, 2, 1, 1, Qt::AlignRight);

  setLayout(layout);

  connect(m_btnClear, SIGNAL(clicked()), this, SLOT(clear()));
  connect(m_btnCommit, SIGNAL(clicked()), this, SLOT(prepareCommit()));
}

void KeywordEditor::prepareCommit() {
  int id = getKeyId();
  QJsonObject obj;
  if (id > 0)
    obj.insert("ci_id", QJsonValue(id));

  obj.insert("ci_name", QJsonValue(getKeyword()));
  obj.insert("ci_company_usage", QJsonValue(getBookUsage()));

  if (id == 0)
    emit insertKeyword(obj);
  else
    emit updateKeyword(obj);
}

void KeywordEditor::clear() {
  m_id->setValue(0);
  m_keyword->clear();
  m_booksUsage->setChecked(false);
}

void KeywordEditor::setKeyId(int id) { m_id->setValue(id); }

void KeywordEditor::setKeyword(const QString &keyword) {
  QString txt = keyword.trimmed();
  txt.replace(p_stripRegExp, "");
  m_keyword->setText(txt);
}

void KeywordEditor::setBookUsage(bool b) { m_booksUsage->setChecked(b); }

int KeywordEditor::getKeyId() { return m_id->value(); }

const QString KeywordEditor::getKeyword() { return m_keyword->text(); }

bool KeywordEditor::getBookUsage() { return m_booksUsage->isChecked(); }
