// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailbutton.h"
#include "ainputwidget.h"

#include <QDebug>
#include <QIcon>

namespace AntiquaCRM {

MailButton::MailButton(QWidget *parent) : QPushButton{parent} {
  setIcon(AntiquaApplIcon("mail-message"));
  setText(tr("Mail Messages"));
  setToolTip(tr("Selection for different eMail messages."));
  setEnabled(false);
  setVisible(false);
  m_menu = new QMenu(this);
  setMenu(m_menu);
}

bool MailButton::createMailButtonActions() {
  QString sql("SELECT tb_caller, tb_title FROM ");
  sql.append("ui_template_body WHERE tb_category='email'");
  if (buttons.testFlag(MAIL_ORDER_GROUP)) {
    sql.append(" AND tb_tab='ORDERS'");
  } else if (buttons.testFlag(MAIL_CUSTOMER_GROUP)) {
    sql.append(" AND tb_tab='CUSTOMERS'");
  } else {
    qWarning("Unknown Mailbutton group!");
    return false;
  }
  sql.append(" AND tb_activ=true ORDER BY (tb_title,tb_subject);");

  AntiquaCRM::ASqlCore pgsql(this);
  QSqlQuery q = pgsql.query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString title = q.value("tb_title").toString();
      QAction *ac = m_menu->addAction(icon(), title);
      ac->setObjectName(q.value("tb_caller").toString());
      connect(ac, SIGNAL(triggered()), this, SLOT(setMailAction()));
    }
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else if (!pgsql.lastError().isEmpty()) {
    qDebug() << Q_FUNC_INFO << sql << pgsql.lastError();
  }
#endif
  return false;
}

void MailButton::setMailAction() {
  emit sendMailAction(sender()->objectName());
}

void MailButton::hasMailAddress(bool b) {
  setEnabled(b);
  setVisible(b);
}

void MailButton::setSections(AntiquaCRM::MailGroups flags) {
  buttons = flags;
  m_menu->clear();

  // Wenn gesetzt, hier Aussteigen.
  if (flags.testFlag(NO_MAIL_GROUP)) {
    setEnabled(false);
    return;
  }

  if (!createMailButtonActions())
    return;

  emit sendSectionChanged();
}

AntiquaCRM::MailGroups MailButton::getSections() { return buttons; }

} // namespace AntiquaCRM
