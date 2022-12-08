// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailbutton.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QIcon>

MailButton::MailButton(QWidget *parent) : QPushButton{parent} {
  setIcon(QIcon(":icons/user_identity.png"));
  setText(tr("Mail Messages"));
  setToolTip(tr("Selection for different eMail messages."));
  setEnabled(false);
  setVisible(false);
  m_menu = new QMenu(this);
  m_menu->setObjectName("mailbutton_menue");
  setMenu(m_menu);
}

bool MailButton::createMailButtonActions() {
  QString sql("SELECT tb_caller,tb_title FROM ui_template_body");
  if (sections.testFlag(Orders)) {
    sql.append(" WHERE tb_category='email' AND tb_tab='ORDERS';");
  } else if (sections.testFlag(Customers)) {
    sql.append(" WHERE tb_category='email' AND tb_tab='CUSTOMERS';");
  } else {
    qWarning("Unknown Mailbutton group!");
    return false;
  }

  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QString title = q.value("tb_title").toString();
      QAction *ac = m_menu->addAction(icon(), title);
      ac->setObjectName(q.value("tb_caller").toString());
      connect(ac, SIGNAL(triggered()), this, SLOT(setMailAction()));
    }
    return true;
  }
  return false;
}

void MailButton::setMailAction() {
  emit sendMailAction(sender()->objectName());
}

void MailButton::hasMailAddress(bool b) {
  setEnabled(b);
  setVisible(b);
}

void MailButton::setSections(MailButton::Sections flags) {
  sections = flags;
  m_menu->clear();

  // Wenn gesetzt, hier Aussteigen.
  if (flags.testFlag(NoButton)) {
    setEnabled(false);
    return;
  }

  if (!createMailButtonActions())
    return;

  emit sendSectionChanged();
}

MailButton::Sections MailButton::getSections() { return sections; }
