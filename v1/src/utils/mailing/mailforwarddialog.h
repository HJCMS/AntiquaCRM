// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILING_MAILFORWARDDIALOG_H
#define MAILING_MAILFORWARDDIALOG_H

#include <AntiquaCRM>
#include <AntiquaTemplates>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QRegularExpression>
#include <QStatusBar>
#include <QWidget>

class MailBody;
class MailTemplateKeys;
class MailAttachments;

/**
 * @brief E-Mail Nachrichten für CLI erstellen.
 * https://docs.microsoft.com/de-de/appcenter/cli/
 */
class MailForwardDialog final : public QDialog {
  Q_OBJECT

private:
  int p_customerId = -1;
  int p_orderId = -1;
  int p_gender = -1;
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  MailTemplateKeys *m_keys;
  MessageSelecter *m_selecter;
  MailBody *m_mailBody;
  MailAttachments *m_attachmentBar;
  QDialogButtonBox *m_btnBox;
  QPushButton *m_btnMail;
  QStatusBar *m_statusBar;
  QRegularExpression p_expression = QRegularExpression("(@[A-Z_]+@)+");

  bool createMailSelect(const QString &tpl);
  bool queryDataFields(int customerId);

private Q_SLOTS:
  void setMailCommand();
  void setBody(const QString &body);
  void setAttachment(bool);

public:
  explicit MailForwardDialog(QWidget *parent = nullptr);
  void setOrderId(int orderId);
  int exec(int customerId, const QString &tpl);
};

#endif // MAILING_MAILFORWARDDIALOG_H
