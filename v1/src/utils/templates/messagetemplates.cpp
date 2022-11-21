// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagetemplates.h"
#include "messageeditor.h"
#include "messagekeywordlist.h"
#include "messageselecter.h"

#include <QIcon>
#include <QLayout>

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
  btn_save->setIcon(QIcon(":icons/action_save.png"));
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

const QString MessageTemplates::buildTitle(const QString &key) const {
  QString title = key.trimmed().toLower();
  title.replace("_"," ");
  QStringList array = title.split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

bool MessageTemplates::createCompanySection() {
  AntiquaCRM::ASqlFiles file("query_company_data");
  if (file.openTemplate()) {
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        QJsonObject obj;
        obj.insert("title", QJsonValue(q.value("ac_info").toString()));
        obj.insert("key", QJsonValue(q.value("ac_class").toString()));
        m_keysList->addKey("COMPANY", obj);
      }
    } else if (!m_sql->lastError().isEmpty()) {
      m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
      return false;
    }
  }
  return true;
}

bool MessageTemplates::createSqlSection() {
  AntiquaCRM::ASqlFiles file("query_template_keys");
  if (file.openTemplate()) {
    file.setWhereClause("tk_key IS NOT NULL");
    QSqlQuery q = m_sql->query(file.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        QJsonObject obj;
        obj.insert("title", QJsonValue(q.value("tk_title").toString()));
        obj.insert("key", QJsonValue(q.value("tk_key").toString()));
        m_keysList->addKey(q.value("tk_type").toString(), obj);
      }
    } else if (!m_sql->lastError().isEmpty()) {
      m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
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
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
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
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << sql << m_sql->lastError();
#endif
  }
}

int MessageTemplates::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  if (!createCompanySection())
    return QDialog::Rejected;

  if (!createSqlSection())
    return QDialog::Rejected;

  if (!createSelecters())
    return QDialog::Rejected;

  m_keysList->expandAll();

  return QDialog::exec();
}
