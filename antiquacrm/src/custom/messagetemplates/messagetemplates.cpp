// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagetemplates.h"
#include "messageeditor.h"
#include "messagekeywordlist.h"
#include "messageselecter.h"

#include <QLayout>
#include <QtCore>

MessageTemplates::MessageTemplates(QWidget *parent) : QDialog{parent} {
  setObjectName("message_templates_dialog");
  setMinimumSize(600, 500);
  setSizeGripEnabled(true);
  setWhatsThis(tr("in this Area"));

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new MessageSelecter(this, true);
  layout->addWidget(m_toolBar); // #0

  m_splitter = new QSplitter(Qt::Horizontal, this);
  layout->addWidget(m_splitter); // #1

  m_editor = new MessageEditor(m_splitter);
  m_splitter->insertWidget(0, m_editor);

  m_keysList = new MessageKeywordList(m_splitter);
  m_splitter->insertWidget(1, m_keysList);

  m_btnBox = new QDialogButtonBox(
      (QDialogButtonBox::Close | QDialogButtonBox::Save), this);
  btn_save = m_btnBox->button(QDialogButtonBox::Save);
  btn_save->setIcon(myIcon("filesave"));
  layout->addWidget(m_btnBox); // #2

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar); // #3

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_toolBar, SIGNAL(sendBody(const QString &)), m_editor,
          SLOT(setBody(const QString &)));

  connect(btn_save, SIGNAL(clicked()), this, SLOT(setSqlQuery()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
}

const QString MessageTemplates::querySection(const QString &name) const {
  QString sql("SELECT tk_key AS key, tk_title AS title");
  sql.append(" FROM ui_template_keys WHERE tk_type='");
  sql.append(name + "' ORDER BY tk_key;");
  return sql;
}

bool MessageTemplates::createSectionTree() {
  // "SQL"|"TXT"|"PATTERN"
  QMap<QString, QString> map;
  map.insert("SQL", tr("Orders system"));
  map.insert("TXT", tr("Provider orders"));
  QMapIterator<QString, QString> it(map);
  while (it.hasNext()) {
    it.next();
    QString sql = querySection(it.key());
    QSqlQuery q = m_sql->query(sql);
    // qDebug() << sql << q.size();
    if (q.size() > 0) {
      while (q.next()) {
        QJsonObject obj;
        obj.insert("title", QJsonValue(q.value("title").toString()));
        obj.insert("key", QJsonValue(q.value("key").toString()));
        m_keysList->addKey(it.value(), obj);
      }
    } else if (!m_sql->lastError().isEmpty()) {
      m_statusBar->showMessage(tr("an error occurred"));
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
      return false;
    }
  }
  return true;
}

bool MessageTemplates::createSelecters() {
  QString sql("SELECT * FROM ui_template_body ORDER BY tb_title;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QList<MessageCaller> list;
    while (q.next()) {
      MessageCaller cl;
      cl.setCaller(q.value("tb_caller").toString());
      cl.setTitle(q.value("tb_title").toString());
      cl.setSubject(q.value("tb_subject").toString());
      cl.setBody(q.value("tb_message").toString());
      cl.setGender(q.value("tb_gender").toInt());
      cl.setAttachment(q.value("tb_attachment").toBool());
      list.append(cl);
    }
    if (list.count() > 0)
      m_toolBar->setSelecters(list);

  } else if (!m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("an error occurred"));
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    return false;
  }
  return true;
}

void MessageTemplates::setSqlQuery() {
  QString tb_caller = m_toolBar->getCaller();
  if (tb_caller.isEmpty()) {
    m_statusBar->showMessage(tr("The caller field is mandatory!"));
    return;
  }

  QString tb_title = m_toolBar->getTitle();
  if (tb_title.isEmpty()) {
    m_statusBar->showMessage(tr("The title cannot be empty!"));
    return;
  }

  QString tb_subject = m_toolBar->getSubject();
  if (tb_subject.isEmpty()) {
    m_statusBar->showMessage(tr("The Subject cannot be empty!"));
    return;
  }

  QString tb_message = m_editor->toPlainText();
  if (tb_message.isEmpty()) {
    m_statusBar->showMessage(tr("Empty body text is not accepted!"));
    return;
  }

  QString tb_gender = QString::number(m_toolBar->getGender());
  QString tb_attachment = (m_toolBar->hasAttachment()) ? "true" : "false";

  QString sql("SELECT * FROM ui_template_body WHERE tb_title='");
  sql.append(tb_title + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    sql = QString("UPDATE ui_template_body SET");
    sql.append(" tb_caller='" + tb_caller + "',");
    sql.append(" tb_message='" + tb_message + "',");
    sql.append(" tb_subject='" + tb_subject + "',");
    sql.append(" tb_gender=" + tb_gender + ",");
    sql.append(" tb_attachment=" + tb_attachment);
    sql.append(" WHERE tb_title='" + tb_title + "';");
  } else {
    sql = QString("INSERT INTO ui_template_body");
    sql.append(" (tb_caller,tb_title,tb_message,tb_gender,tb_subject)");
    sql.append(" VALUES (");
    sql.append(" '" + tb_caller + "','" + tb_title + "',");
    sql.append(" '" + tb_message + "'," + tb_gender + ",");
    sql.append(" '" + tb_subject + "'," + tb_attachment + ");");
  }

  q = m_sql->query(sql);
  if (m_sql->lastError().isEmpty()) {
    createSelecters();
    m_statusBar->showMessage(tr("Save data successfully!"), (1000 * 6));
  } else {
    m_statusBar->showMessage(tr("an error occurred"));
    qDebug() << Q_FUNC_INFO << sql << m_sql->lastError();
  }
}

int MessageTemplates::exec() {
  m_sql = new HJCMS::SqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  if (!createSectionTree())
    return QDialog::Rejected;

  if (!createSelecters())
    return QDialog::Rejected;

  return QDialog::exec();
}
