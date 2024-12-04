// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articledescwordswidget.h"
#include "articledescwordstable.h"

#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>

ArticleDescWordsWidget::ArticleDescWordsWidget(QWidget* parent) : QWidget{parent} {
  setObjectName("article_descriptions_editor");

  QGridLayout* m_layout = new QGridLayout(this);
  // setStyleSheet("border:1px solid red;");

  m_table = new ArticleDescWordsTable(this);
  m_layout->addWidget(m_table, 0, 0, 1, 4);

  m_id = new AntiquaCRM::SerialId(this);
  m_id->setObjectName("aes_id");
  m_id->setBuddyLabel(tr("Id"));
  m_layout->addWidget(m_id, 1, 0, 1, 1);

  QPushButton* btn_clear = new QPushButton(tr("Clear"));
  m_layout->addWidget(btn_clear, 1, 1, 1, 1);

  m_type = new AntiquaCRM::SectionSelecter(this);
  m_type->setObjectName("aes_type");
  m_type->setBuddyLabel(tr("Section"));
  m_layout->addWidget(m_type, 1, 2, 1, 1);

  m_lang = new AntiquaCRM::SelectLanguage(this);
  m_lang->setObjectName("aes_lang");
  m_lang->setBuddyLabel(tr("Language"));
  m_layout->addWidget(m_lang, 1, 3, 1, 1);

  m_title = new AntiquaCRM::TextLine(this);
  m_title->setObjectName("aes_title");
  m_title->setBuddyLabel(tr("Menu entry"));
  m_layout->addWidget(m_title, 2, 0, 1, 4);

  m_data = new AntiquaCRM::TextField(this);
  m_data->setObjectName("aes_data");
  m_layout->addWidget(m_data, 3, 0, 1, 4);

  setLayout(m_layout);

  connect(btn_clear, SIGNAL(clicked()), SLOT(clearInput()));
  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));
  connect(m_table, SIGNAL(sendDeleteEntry(qint64)), SLOT(deleteEntry(qint64)));
  connect(m_table, SIGNAL(doubleClicked(QModelIndex)), SLOT(itemSelected(QModelIndex)));
}

void ArticleDescWordsWidget::clearInput() {
  QList<AntiquaCRM::AInputWidget*> _list = findChildren<AntiquaCRM::AInputWidget*>(QString());
  foreach (AntiquaCRM::AInputWidget* w, _list) {
    w->reset();
  }
}

void ArticleDescWordsWidget::openEntry(qint64 id) {
  QString _sql("SELECT * FROM " + m_table->tableName() + " ");
  _sql.append("WHERE aes_id=" + QString::number(id) + ";");
  AntiquaCRM::ASqlCore p_sql(this);
  if (p_sql.open()) {
    QSqlQuery _q = p_sql.query(_sql);
    QSqlRecord _r = _q.record();
    while (_q.next()) {
      // qDebug() << Q_FUNC_INFO << _sql;
      for (int c = 0; c < _r.count(); c++) {
        QString _n = _r.field(c).name();
        AntiquaCRM::AInputWidget* m_w = findChild<AntiquaCRM::AInputWidget*>(_n);
        if (m_w != nullptr) {
          m_w->setValue(_q.value(_n));
        }
      }
    }
  }
}

void ArticleDescWordsWidget::deleteEntry(qint64 id) {
  QString _sql("DELETE FROM " + m_table->tableName() + " WHERE aes_id=");
  _sql.append(QString::number(id) + ";");
  AntiquaCRM::ASqlCore p_sql(this);
  if (p_sql.open()) {
    p_sql.query(_sql);
    if (p_sql.lastError().isEmpty()) {
      m_table->setQuery();
    } else {
      QString info = p_sql.lastError();
      QMessageBox* m_b = new QMessageBox(QMessageBox::Critical, // Critical
                                         tr("Database Error"), info, QMessageBox::Ok, this);
      m_b->setInformativeText(_sql);
      m_b->exec();
    }
  }
}

void ArticleDescWordsWidget::itemSelected(const QModelIndex& index) {
  qint64 _id = m_table->getItemId(index);
  if (_id > 0)
    openEntry(_id);
}

void ArticleDescWordsWidget::sqlCommit() {
  qint64 _id = m_id->getValue().toInt();  // aes_id
  int _type = m_type->getValue().toInt(); // aes_type
  if (_type < 1) {
    m_type->setFocus();
    emit sendStatusMessage(tr("Section is not set!"));
    return;
  }

  QString _lang = m_lang->getValue().toString(); // aes_lang
  if (_lang.length() < 2 || _lang == "xx") {
    m_lang->setFocus();
    emit sendStatusMessage(tr("Missing language!"));
    return;
  }

  QString _title = m_title->getValue().toString().remove("'"); // aes_title
  if (_title.length() < 3) {
    m_title->setFocus();
    emit sendStatusMessage(tr("Missing title!"));
    return;
  }

  QString _data = m_data->getValue().toString().remove("'"); // aes_data
  if (_data.length() < 4) {
    m_data->setFocus();
    emit sendStatusMessage(tr("Description is to small!"));
    return;
  }

  QString _sql;
  if (_id > 0) {
    _sql.append("UPDATE " + m_table->tableName() + " SET ");
    _sql.append("aes_type=" + QString::number(_type) + ", ");
    _sql.append("aes_lang='" + _lang + "', ");
    _sql.append("aes_title='" + _title + "', ");
    _sql.append("aes_data='" + _data + "' ");
    _sql.append("WHERE aes_id=" + QString::number(_id) + ";");
  } else {
    _sql.append("INSERT INTO " + m_table->tableName() + " ");
    _sql.append("(aes_type,aes_lang,aes_title,aes_data) ");
    _sql.append(" VALUES ");
    _sql.append("(" + QString::number(_type) + ",");
    _sql.append("'" + _lang + "',");
    _sql.append("'" + _title + "',");
    _sql.append("'" + _data + "'");
    _sql.append(");");
  }
  qDebug() << Q_FUNC_INFO << _sql;
  // emit sendCloseDialog(true);
}

bool ArticleDescWordsWidget::sqlQuery() {
  return m_table->setQuery();
};
