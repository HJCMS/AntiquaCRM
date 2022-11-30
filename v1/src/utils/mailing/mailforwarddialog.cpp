// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailforwarddialog.h"
#include "mailattachments.h"
#include "mailbody.h"
#include "mailcommand.h"
#include "mailtemplatekeys.h"

#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QTextCursor>
#include <QTimer>

MailForwardDialog::MailForwardDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("create_mail_forward_dialog");
  setWindowTitle(tr("Mail Templates"));
  setMinimumSize(QSize(650, 450));
  setSizeGripEnabled(true);

  m_cfg = new AntiquaCRM::ASettings(this);
  m_keys = new MailTemplateKeys(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_mailSubject = new QLabel(this);
  m_mailSubject->setObjectName("tb_subject");
  layout->addWidget(m_mailSubject);

  m_mailBody = new MailBody(this);
  m_mailBody->setObjectName("tb_message");
  layout->addWidget(m_mailBody);

  m_attachmentBar = new MailAttachments(this);
  m_attachmentBar->setObjectName("tb_attachment");
  layout->addWidget(m_attachmentBar);

  m_btnBox = new QDialogButtonBox(this);

  btn_email = new QPushButton(tr("Mail"), m_btnBox);
  btn_email->setIcon(QIcon(":icons/user_identity.png"));
  btn_email->setEnabled(false);
  btn_email->setToolTip(tr("Start eMail command"));
  m_btnBox->addButton(btn_email, QDialogButtonBox::ActionRole);

  btn_close = new QPushButton(tr("Close"), m_btnBox);
  btn_close->setIcon(QIcon("://icons/action_quit.png"));
  btn_close->setToolTip(tr("Closing this dialog"));
  m_btnBox->addButton(btn_close, QDialogButtonBox::RejectRole);

  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(btn_email, SIGNAL(clicked()), this, SLOT(setMailCommand()));
}

void MailForwardDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    e->setAccepted(false);
    m_statusBar->showMessage(tr("Please use the Finish button!"));
    return;
  }
  QDialog::closeEvent(e);
}

bool MailForwardDialog::createMailSelect(const QString &tpl) {
  QString sql("SELECT * FROM ui_template_body");
  sql.append(" WHERE tb_caller='" + tpl + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    tplData = QJsonObject();
    QSqlRecord r = q.record();
    while (q.next()) {
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        QVariant value = q.value(f.name());
        tplData.insert(f.name(), QJsonValue::fromVariant(value));
      }
    }
    setWindowTitle(tplData.value("tb_title").toString());
    setSubject(tplData.value("tb_subject").toString());
    setBody(tplData.value("tb_message").toString());
    setAttachment(tplData.value("tb_attachment").toBool());
    return true;
  } else if (!m_sql->lastError().isEmpty()) {
    m_statusBar->showMessage(tr("an error occurred"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    return false;
  }
  return false;
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
        QString fn = sf.name();
        QVariant v = q.value(sf.name());
        if (v.isValid() && v.type() == QVariant::String &&
            v.toString().isEmpty())
          continue;

        if (fn == "c_gender") {
          // TODO tplData
          p_gender = v.toInt();
        }

        buffer.insert(fn, v);
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
          QString fn = sf.name();
          QVariant v = q.value(sf.name());
          if (v.isValid() && v.type() == QVariant::String &&
              v.toString().isEmpty())
            continue;

          buffer.insert(fn, v);
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
  connect(cli, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  bool attach = tplData.value("tb_attachment").toBool();
  QString eMail = m_keys->getData("c_email_0").toString();
  cli->setMail(eMail);
  cli->setSubject(m_mailSubject->text());
  cli->setBody(m_mailBody->toPlainText());
  if (attach && m_attachmentBar->exists()) {
    cli->setAttachment(m_attachmentBar->attachment().filePath());
  }
  cli->sendMail();

  QTimer::singleShot(2000, this, SLOT(accept()));
}

void MailForwardDialog::setSubject(const QString &subject) {
  m_mailSubject->setText(subject);
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
  m_mailBody->setFocus();
}

void MailForwardDialog::setAttachment(bool b) {
  int invoice = m_keys->getData("o_invoice_id").toInt();
  if (b && invoice > 0) {
    btn_email->setEnabled(false);
    m_attachmentBar->clear();
    m_attachmentBar->setActive(true);

    QString id = QString::number(invoice).rightJustified(7, '0');
    if (m_attachmentBar->setAttachment(id)) {
      btn_email->setEnabled(true);
    } else {
      m_statusBar->showMessage(tr("Missing Attachment"), 0);
    }
  } else {
    btn_email->setEnabled(false);
    m_attachmentBar->clear();
  }
}

void MailForwardDialog::setOrderId(int orderId) { p_orderId = orderId; }

int MailForwardDialog::exec() {
  qWarning("Please use exec with costumerId and Template");
  return QDialog::Rejected;
}

int MailForwardDialog::exec(int customerId, const QString &tpl) {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  m_keys->run();

  if (!queryDataFields(customerId))
    return QDialog::Rejected;

  if (!createMailSelect(tpl))
    return QDialog::Rejected;

  return QDialog::exec();
}
