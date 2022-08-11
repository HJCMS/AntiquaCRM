// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailforwarddialog.h"
#include "mailbody.h"
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

  m_selecter = new MessageSelecter(this); // #0
  layout->addWidget(m_selecter);

  m_mailBody = new MailBody(this); // #1
  layout->addWidget(m_mailBody);

  m_btnBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
  layout->addWidget(m_btnBox);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->addWidget(m_statusBar);

  layout->setStretch(1, 1);
  setLayout(layout);

  connect(m_selecter, SIGNAL(sendBody(const QString &)), this,
          SLOT(setBody(const QString &)));

  connect(m_btnBox, SIGNAL(rejected()), this, SLOT(reject()));
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

void MailForwardDialog::setSubject(const QString &subject) {
  Q_UNUSED(subject);
}

void MailForwardDialog::setBody(const QString &body) {
  QString buffer(body);
  QRegularExpressionMatchIterator i = p_expression.globalMatch(body);
  while (i.hasNext()) {
    QRegularExpressionMatch m = i.next();
    if (m.isValid()) {
      QString from = m.captured(0);
      QString to = m_keys->getKey(from);
      qDebug() << Q_FUNC_INFO << from << to;
      if (!to.isEmpty())
        buffer.replace(from, to, Qt::CaseSensitive);
    }
  }
  m_mailBody->setPlainText(buffer);
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
