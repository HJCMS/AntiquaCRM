// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordedit.h"
#include "keywordactions.h"
#include "keywordeditor.h"
#include "keywordtable.h"
#include "myicontheme.h"

#include <QDebug>
#include <QMap>
#include <QVBoxLayout>

KeywordEdit::KeywordEdit(QWidget *parent) : QDialog{parent} {
  setObjectName("keyword_edit_dialog");
  setSizeGripEnabled(true);
  setMinimumSize(550, 500);

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_searchBar = new QToolBar(this);
  m_searchBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_searchLine = new QLineEdit(m_searchBar);
  m_searchBar->addWidget(m_searchLine);
  QAction *ac_search = m_searchBar->addAction(myIcon("search"), tr("Search"));
  layout->addWidget(m_searchBar);

  m_table = new KeywordTable(m_sql->db(), this);
  layout->addWidget(m_table);

  m_editor = new KeywordEditor(this);
  layout->addWidget(m_editor);

  m_actionsBar = new KeywordActions(this);
  layout->addWidget(m_actionsBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(ac_search, SIGNAL(triggered()), this, SLOT(searchKeyword()));
  connect(m_searchLine, SIGNAL(returnPressed()), this, SLOT(searchKeyword()));
  connect(m_table, SIGNAL(sendEditKeyword(int)), this,
          SLOT(queryKeywordEntry(int)));
  connect(m_actionsBar, SIGNAL(sendQuit()), this, SLOT(reject()));
  connect(m_editor, SIGNAL(updateKeyword(const QJsonObject &)), this,
          SLOT(updateKeywordEntry(const QJsonObject &)));
  connect(m_editor, SIGNAL(insertKeyword(const QJsonObject &)), this,
          SLOT(insertKeywordEntry(const QJsonObject &)));
}

bool KeywordEdit::initBooklooker() {
  QString sql("SELECT * FROM ref_booklooker ORDER BY bl_section ASC;");
  QSqlQuery q = m_sql->query(sql);
  QMap<QString,int> map;
  if (q.size() > 0) {
    while (q.next()) {
      map.insert(q.value("bl_section").toString(),q.value("bl_id").toInt());
    }
    m_editor->setBooklookerBox(map);
    return true;
  }
  return false;
}

void KeywordEdit::searchKeyword() {
  QString search = m_searchLine->text().trimmed();
  if (!search.isEmpty())
    m_table->searchKeyword(search);
}

void KeywordEdit::queryKeywordEntry(int id) {
  QString sql("SELECT * FROM categories_intern");
  sql.append(" WHERE ci_id=");
  sql.append(QString::number(id));
  sql.append(" ORDER BY ci_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      if (q.value("ci_id").toInt() == id) {
        m_editor->clear();
        m_editor->setKeyId(id);
        m_editor->setKeyword(q.value("ci_name").toString());
        m_editor->setBookUsage(q.value("ci_company_usage").toBool());
        m_editor->setBooklooker(q.value("ci_booklooker_id").toInt());
        break;
      }
    }
    m_actionsBar->statusMessage(tr("Database query success!"));
  } else {
    qDebug() << m_sql->lastError();
  }
}

void KeywordEdit::updateKeywordEntry(const QJsonObject &obj) {
  int id = obj.value("ci_id").toInt();
  if (id < 1)
    return;

  QString usage = obj.value("ci_company_usage").toBool() ? "true" : "false";
  int bl_id = obj.value("ci_booklooker_id").toInt();
  QString sql("UPDATE categories_intern SET ");
  sql.append("ci_name='" + obj.value("ci_name").toString() + "'");
  sql.append(",ci_company_usage=" + usage);
  sql.append(",ci_booklooker_id=" + QString::number(bl_id));
  sql.append(" WHERE ci_id=" + QString::number(id));
  sql.append(";");
  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    m_editor->clear();
    m_table->refresh();
    m_actionsBar->statusMessage(tr("Database query success!"));
  } else {
    qDebug() << m_sql->lastError();
  }
}

void KeywordEdit::insertKeywordEntry(const QJsonObject &obj) {
  QString ci_name = obj.value("ci_name").toString().trimmed();
  if (ci_name.isEmpty()) {
    m_actionsBar->statusMessage(tr("Missing Keyword!"));
    return;
  }
  QString bcu = obj.value("ci_company_usage").toBool() ? "true" : "false";
  int bl_id = obj.value("ci_booklooker_id").toInt();
  QString sql("INSERT INTO categories_intern");
  sql.append(" (ci_name,ci_company_usage,ci_booklooker_id)");
  sql.append(" VALUES ('" + ci_name + "'");
  sql.append("," + bcu + "," + QString::number(bl_id) + ");");
  m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    m_editor->clear();
    m_table->refresh();
    m_actionsBar->statusMessage(tr("Database query success!"));
  } else {
    qDebug() << m_sql->lastError();
  }
}

int KeywordEdit::exec() {
  if (m_table != nullptr)
    m_table->refresh();

  initBooklooker();

  return QDialog::exec();
}
