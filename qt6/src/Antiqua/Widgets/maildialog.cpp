// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "maildialog.h"
#include "antiquaicon.h"
#include "private/mailattachments.h"
#include "private/mailcommand.h"
#include "private/mailtemplatekeys.h"

#include <QDebug>
#include <QLayout>

namespace AntiquaCRM {

MailDialog::MailDialog(QWidget *parent) : QDialog{parent} {
  setObjectName("create_mail_forward_dialog");
  setWindowTitle(tr("Mailler"));
  setMinimumSize(QSize(650, 500));
  setSizeGripEnabled(true);

  m_sql = nullptr;
  m_cfg = new AntiquaCRM::ASettings(this);
  m_keys = new AntiquaCRM::MailTemplateKeys(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  // row 0
  m_subject = new AntiquaCRM::ALabel(this);
  m_subject->setObjectName("tb_subject");
  layout->addWidget(m_subject);

  // row 1
  m_body = new AntiquaCRM::TextField(this);
  m_body->setObjectName("tb_message");
  layout->addWidget(m_body);
  layout->setStretch(1, 1);

  // row 2
  m_attachments = new AntiquaCRM::MailAttachments(this);
  m_attachments->setObjectName("tb_attachment");
  layout->addWidget(m_attachments);

  // row 3
  m_btnBox = new QDialogButtonBox(Qt::Horizontal, this);
  btn_email = new QPushButton(tr("Mail"), m_btnBox);
  btn_email->setIcon(AntiquaCRM::antiquaIcon("mail-send"));
  btn_email->setToolTip(tr("Start eMail command"));
  m_btnBox->addButton(btn_email, QDialogButtonBox::ActionRole);
  btn_close = new QPushButton(tr("Close"), m_btnBox);
  btn_close->setIcon(AntiquaCRM::antiquaIcon("dialog-close"));
  btn_close->setToolTip(tr("Closing this dialog"));
  m_btnBox->addButton(btn_close, QDialogButtonBox::RejectRole);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);
  setLayout(layout);

  connect(m_keys, SIGNAL(sendSuccess()), SLOT(setRecipientData()));
  connect(m_keys, SIGNAL(sendNoResult()), SLOT(setFailed()));
  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(btn_email, SIGNAL(clicked()), this, SLOT(setMailCommand()));
}

void MailDialog::closeEvent(QCloseEvent *event) {
  if (event->type() == QEvent::Close) {
    event->setAccepted(false);
    m_statusBar->showMessage(tr("Please use the Close button!"));
    return;
  }
  QDialog::closeEvent(event);
}

bool MailDialog::selectTemplate(const QString &caller) {
  QString sql("SELECT * FROM ui_template_body");
  sql.append(" WHERE tb_caller='" + caller + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    templateData = QJsonObject();
    QSqlRecord r = q.record();
    while (q.next()) {
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        QVariant value = q.value(f.name());
        templateData.insert(f.name(), QJsonValue::fromVariant(value));
      }
    }
    setWindowTitle(templateData.value("tb_title").toString());
    setSubject(templateData.value("tb_subject").toString());
    setBody(templateData.value("tb_message").toString());
    setAttachment();
    m_statusBar->showMessage(tr("Template loaded."));
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

const QJsonObject MailDialog::articleObject(const QVariant &value) {
  QJsonDocument doc = QJsonDocument::fromJson(value.toByteArray());
  return doc.object();
}

void MailDialog::setRecipientData() {
  if (p_customerId < 0 || p_tb_caller.isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << p_customerId << p_tb_caller;
#endif
    return;
  }

  QMap<QString, QVariant> _map;
  _map.insert("c_id", p_customerId);

  QString sql("SELECT * FROM customers");
  sql.append(" WHERE c_id=" + QString::number(p_customerId));
  sql.append(" ORDER BY c_id;");

  QSqlQuery _q = m_sql->query(sql);
  if (_q.size()) {
    QSqlRecord _record = m_sql->record("customers");
    while (_q.next()) {
      for (int r = 0; r < _record.count(); r++) {
        QString _name = _record.field(r).name();
        QVariant _value = _q.value(_name);
        if (_value.isValid() &&
            (_value.metaType().id() == QMetaType::QString) &&
            _value.toString().isEmpty())
          continue;

        if (_name == "c_gender") {
          // TODO tplData
          p_gender = _value.toInt();
        }
        _map.insert(_name, _value);
      }
    }
  }
  _q.clear();

  // BEGIN Orders Request
  if (p_orderId > 0) {
    _map.insert("o_id", p_orderId);
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
          if (v.isValid() && (v.metaType().id() == QMetaType::QString) &&
              v.toString().isEmpty())
            continue;

          _map.insert(fn, v);
        }
      }
    }

    // Article Content
    AntiquaCRM::ASqlFiles tplArticle("query_tpl_mail_article");
    if (tplArticle.openTemplate()) {
      tplArticle.setWhereClause("a_order_id=" + QString::number(p_orderId));
      sql = tplArticle.getQueryContent();
      QSqlQuery q = m_sql->query(sql);
      if (q.size()) {
        QStringList list;
        QJsonArray articles;
        while (q.next()) {
          list << AntiquaCRM::AUtil::zerofill(q.value("a_article_id").toInt());
          articles.append(articleObject(q.value("a_article_display")));
        }
        _map.insert("a_article_id", list.join(","));
        _map.insert("a_article_list", articles);
      } else {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
        qWarning("SQL error occurred in mail article templates");
#endif
      }
    }
  }
  // END

  m_keys->appendData(_map);

  selectTemplate(p_tb_caller);
}

void MailDialog::setFailed() {
  qWarning("No Template keys fetched!");
  m_statusBar->showMessage(tr("No Template data loaded!"));
}

void MailDialog::setMailCommand() {
  MailCommand *cli = new MailCommand(this);
  cli->setObjectName("start_mail_command");
  connect(cli, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  QString eMail = m_keys->getData("c_email_0").toString();
  cli->setSubject(m_subject->text());
  cli->setBody(m_body->getValue().toString());
  if (m_attachments->exists()) {
    cli->setAttachment(m_attachments->attachment().filePath());
  }
  if (!cli->setMail(eMail))
    return; // invalid eMail

  cli->sendMail();
  QTimer::singleShot(2000, this, SLOT(accept()));
}

void MailDialog::setSubject(const QString &txt) { m_subject->setText(txt); }

void MailDialog::setBody(const QString &txt) {
  QString buffer(txt);
  QRegularExpressionMatchIterator i = p_regexp.globalMatch(txt);
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
  m_body->setValue(buffer);
  m_body->setFocus();
}

void MailDialog::setAttachment() {
  int _invoice = m_keys->getData("o_invoice_id").toInt();
  bool _required = templateData.value("tb_attachment").toBool();
  if (_required && _invoice > 0) {
    m_attachments->clear();
    m_attachments->setActive(true);

    QString id = AntiquaCRM::AUtil::zerofill(_invoice);
    if (m_attachments->setAttachment(id)) {
      btn_email->setEnabled(true);
    } else {
      btn_email->setEnabled(false);
      m_statusBar->showMessage(tr("Missing Attachment"), 0);
    }
  } else {
    m_attachments->clear();
  }
}

int MailDialog::exec() {
  qWarning("Do not use a Mailler dialog without options set!");
  return QDialog::Rejected;
}

int MailDialog::exec(const QJsonObject &data) {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return QDialog::Rejected;

  p_orderId = data.value("CRM_ORDER_ID").toInteger(0);
  p_customerId = data.value("CRM_CUSTOMER_ID").toInteger(0);
  if (p_customerId < 1) {
    qWarning("No Customer id found!");
    return QDialog::Rejected;
  }

  p_tb_caller = data.value("tb_caller").toString();
  if (p_tb_caller.isEmpty()) {
    qWarning("No template found!");
    return QDialog::Rejected;
  }

  // Initial Template data
  m_keys->run();

  return QDialog::exec();
}

} // namespace AntiquaCRM
