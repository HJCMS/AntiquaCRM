// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagetemplates.h"
#include "messageeditor.h"
#include "messagekeywordlist.h"
#include "messageselecter.h"

#include <QtCore>
#include <QtWidgets>

MessageTemplates::MessageTemplates(QWidget *parent) : QDialog{parent} {
  setObjectName("message_templates_dialog");
  setMinimumSize(600, 500);
  setSizeGripEnabled(true);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new MessageSelecter(this);
  layout->addWidget(m_toolBar); // #0

  m_splitter = new QSplitter(Qt::Horizontal, this);
  layout->addWidget(m_splitter); // #1

  m_editor = new MessageEditor(m_splitter);
  m_splitter->insertWidget(0, m_editor);

  m_keysList = new MessageKeywordList(m_splitter);
  m_splitter->insertWidget(1, m_keysList);

  m_btnBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_btnBox); // #2

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar); // #3

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
}

const QString MessageTemplates::querySection(const QString &name) const {
  QString sql("SELECT tk_key AS key, tk_json AS json");
  sql.append(" FROM ui_template_keys WHERE tk_type='");
  sql.append(name + "' ORDER BY tk_key;");
  return sql;
}

int MessageTemplates::exec() {
  m_sql = new HJCMS::SqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  // "SQL"|"TXT"|"PATTERN"
  QMap<QString, QString> map;
  map.insert("SQL", tr("Database"));
  map.insert("TXT", tr("Text"));
  map.insert("PATTERN", tr("Patterns"));

  QMapIterator<QString, QString> it(map);
  while (it.hasNext()) {
    it.next();
    QString sql = querySection(it.key());
    QSqlQuery q = m_sql->query(sql);
    qDebug() << sql << q.size();
    if (q.size() > 0) {
      while (q.next()) {
        QByteArray array = q.value("json").toByteArray();
        QJsonObject obj = QJsonDocument::fromJson(array).object();
        if (!obj.isEmpty())
          m_keysList->addKey(it.value(), q.value("key").toString(), obj);
      }
    } else if (!m_sql->lastError().isEmpty()) {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
      return QDialog::Rejected;
    }
  }

  return QDialog::exec();
}
