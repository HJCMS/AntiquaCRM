// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailforwarddialog.h"
#include "mailattachments.h"
#include "mailbody.h"
#include "mailcommand.h"
#include "mailtemplatekeys.h"

#include <QDebug>
#include <QLayout>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextCursor>

MailForwardDialog::MailForwardDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("create_mail_forward_dialog");
  setWindowTitle(tr("Mail Templates") + "[*]");
  setMinimumSize(QSize(500, 500));
  setSizeGripEnabled(true);

  m_cfg = new ApplSettings(this);

  m_keys = new MailTemplateKeys(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_selecter = new MessageSelecter(this);
  layout->addWidget(m_selecter);

  m_attachmentBar = new MailAttachments(this);
  layout->addWidget(m_attachmentBar);

  // Stretch #2
  m_mailBody = new MailBody(this);
  layout->addWidget(m_mailBody);

  m_btnBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  m_btnMail = new QPushButton(tr("Mail"), m_btnBox);
  m_btnMail->setIcon(myIcon("identity"));
  m_btnMail->setEnabled(false);
  m_btnMail->setToolTip(tr("Start eMail command"));
  m_btnBox->addButton(m_btnMail, QDialogButtonBox::ActionRole);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(2, 1);
  setLayout(layout);

  connect(m_selecter, SIGNAL(sendBody(const QString &)), this,
          SLOT(setBody(const QString &)));

  connect(m_selecter, SIGNAL(attachmentChanged(bool)), this,
          SLOT(setAttachment(bool)));

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(m_btnMail, SIGNAL(clicked()), this, SLOT(setMailCommand()));
}

bool MailForwardDialog::createMailSelect(const QString &tpl) {
  QString sql("SELECT * FROM ui_template_body");
  sql.append(" WHERE tb_caller='" + tpl + "'");
  sql.append(" ORDER BY tb_title;");
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
      m_selecter->setSelecters(list);

  } else if (!m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("an error occurred"));
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    return false;
  }
  return true;
}

bool MailForwardDialog::queryDataFields(int customerId) {
  p_customerId = customerId;
  QMap<QString, QVariant> buffer;
  buffer.insert("c_id", customerId);
  QString sql("SELECT * FROM customers");
  sql.append(" WHERE c_id=" + QString::number(customerId));
  sql.append(" ORDER BY c_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size()) {
    QSqlRecord rec = m_sql->record("customers");
    while (q.next()) {
      for (int r = 0; r < rec.count(); r++) {
        QSqlField sf = rec.field(r);
        QString fName = sf.name();
        QVariant v = q.value(sf.name());
        if (v.isValid() && v.type() == QVariant::String &&
            v.toString().isEmpty())
          continue;

        if (fName == "c_gender")
          p_gender = v.toInt();

        buffer.insert(fName, v);
      }
    }
  }
  // BEGIN Orders Request
  if (p_orderId > 0) {
    buffer.insert("o_id", p_orderId);
    sql = ("SELECT * FROM inventory_orders");
    sql.append(" WHERE o_id=" + QString::number(p_orderId));
    sql.append(" ORDER BY o_id;");
    QSqlQuery q = m_sql->query(sql);
    if (q.size()) {
      QSqlRecord rec = m_sql->record("inventory_orders");
      while (q.next()) {
        for (int r = 0; r < rec.count(); r++) {
          QSqlField sf = rec.field(r);
          QString fName = sf.name();
          QVariant v = q.value(sf.name());
          if (v.isValid() && v.type() == QVariant::String &&
              v.toString().isEmpty())
            continue;

          buffer.insert(fName, v);
        }
      }
    }

    sql = ("SELECT * FROM article_orders");
    sql.append(" WHERE a_order_id=" + QString::number(p_orderId));
    sql.append(" ORDER BY a_order_id;");
    q = m_sql->query(sql);
    if (q.size()) {
      QSqlRecord rec = m_sql->record("article_orders");
      while (q.next()) {
        for (int r = 0; r < rec.count(); r++) {
          QSqlField sf = rec.field(r);
          QString fName = sf.name();
          QVariant v = q.value(sf.name());
          if (v.isValid() && v.type() == QVariant::String &&
              v.toString().isEmpty())
            continue;

          buffer.insert(fName, v);
        }
      }
    }
  }
  // END

  m_keys->setData(buffer);
  return true;
}

void MailForwardDialog::setMailCommand() {
  MailCommand *cli = new MailCommand(this);
  cli->setObjectName("start_mail_command");

  QString eMail = m_keys->getData("c_email_0").toString();
  cli->setMail(eMail);
  cli->setSubject(m_selecter->getSubject());
  cli->setBody(m_mailBody->toPlainText());
  if (m_selecter->hasAttachment() && m_attachmentBar->exists()) {
    cli->setAttachment(m_attachmentBar->attachment().filePath());
  }
  cli->sendMail();
}

/**
 * @warning Keine Direkte Ersetzung auf TextEdit machen!
 * Wegen Q_SIGNALS wird das ein "Overflow" auslösen!
 */
void MailForwardDialog::setBody(const QString &body) {
  QString buffer(body);
  QRegularExpressionMatchIterator i = p_expression.globalMatch(body);
  while (i.hasNext()) {
    QRegularExpressionMatch m = i.next();
    if (m.isValid()) {
      QString from = m.captured(0);
      QString to = m_keys->convert(from);
      // qDebug() << Q_FUNC_INFO << from << to;
      if (!to.isEmpty())
        buffer.replace(from, to, Qt::CaseSensitive);
    }
  }

  m_mailBody->setPlainText(buffer);
}

void MailForwardDialog::setAttachment(bool b) {
  // immer erst deaktivieren!
  m_btnMail->setEnabled(false);
  m_attachmentBar->setActive(b);
  if (b) {
    int invoice = m_keys->getData("o_invoice_id").toInt();
    if (invoice > 0) {
      QString id = QString::number(invoice).rightJustified(7, '0');
      m_attachmentBar->setAttachment(id);
    }
    if (m_selecter->hasAttachment() && m_attachmentBar->exists()) {
      m_btnMail->setEnabled(true);
    } else {
      m_statusBar->showMessage(tr("Missing Attachment"), 0);
    }
  } else if (!m_selecter->hasAttachment()) {
    m_btnMail->setEnabled(true);
  } else {
    m_attachmentBar->clear();
  }
}

void MailForwardDialog::setOrderId(int orderId) { p_orderId = orderId; }

int MailForwardDialog::exec(int customerId, const QString &tpl) {
  m_sql = new HJCMS::SqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  if (!queryDataFields(customerId))
    return QDialog::Rejected;

  if (!createMailSelect(tpl))
    return QDialog::Rejected;

  m_selecter->setGender(p_gender);

  return QDialog::exec();
}
