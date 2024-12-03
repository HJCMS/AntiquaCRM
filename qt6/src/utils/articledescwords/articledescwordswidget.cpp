// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articledescwordswidget.h"
#include "articledescwordstable.h"

#include <QGridLayout>
#include <QLabel>

ArticleDescWordsWidget::ArticleDescWordsWidget(QWidget* parent) : QWidget{parent} {
  setObjectName("article_descriptions_editor");

  QGridLayout* m_layout = new QGridLayout(this);
  // setStyleSheet("border:1px solid red;");

  m_editTable = new ArticleDescWordsTable(this);
  m_layout->addWidget(m_editTable, 0, 0, 1, 3);

  m_id = new AntiquaCRM::SerialId(this);
  m_id->setObjectName("aes_id");
  m_id->setBuddyLabel(tr("Id"));
  m_layout->addWidget(m_id, 1, 0, 1, 1);

  m_type = new AntiquaCRM::AComboBox(this);
  m_type->setObjectName("aes_type");
  // m_type->setBuddyLabel(tr("Section"));
  m_layout->addWidget(m_type, 1, 1, 1, 1);

  m_lang = new AntiquaCRM::SelectLanguage(this);
  m_lang->setObjectName("aes_lang");
  m_lang->setBuddyLabel(tr("Language"));
  m_layout->addWidget(m_lang, 2, 0, 1, 1);

  m_title = new AntiquaCRM::TextLine(this);
  m_title->setObjectName("aes_title");
  m_title->setBuddyLabel(tr("Menu entry"));
  m_layout->addWidget(m_title, 2, 1, 1, 1);

  m_data = new AntiquaCRM::TextField(this);
  m_data->setObjectName("aes_data");
  m_layout->addWidget(m_data, 3, 0, 1, 3);

  setLayout(m_layout);
}

void ArticleDescWordsWidget::sqlCommit() {
  // emit sendCloseDialog(true);
}

bool ArticleDescWordsWidget::sqlQuery() {
  return m_editTable->setQuery(QString());
};
