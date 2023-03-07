// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILING_MAILFORWARDDIALOG_H
#define MAILING_MAILFORWARDDIALOG_H

#include <AntiquaCRM>
#include <AntiquaTemplates>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QRegularExpression>
#include <QStatusBar>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>

class MailBody;
class MailTemplateKeys;
class MailAttachments;

/**
 * @brief E-Mail Nachrichten für CLI erstellen.
 * @section AntiquaMailing
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
  QJsonObject tplData;
  MailTemplateKeys *m_keys;
  QLabel *m_mailSubject;
  MailBody *m_mailBody;
  MailAttachments *m_attachmentBar;

  QDialogButtonBox *m_btnBox;
  QPushButton *btn_close;
  QPushButton *btn_email;

  QStatusBar *m_statusBar;
  QRegularExpression p_expression = QRegularExpression("(@[A-Z_]+@)+");

  const QJsonObject articleObject(const QVariant &);

  void closeEvent(QCloseEvent *) override;

  bool createMailSelect(const QString &tpl);
  bool queryDataFields(int customerId);

private Q_SLOTS:
  void setMailCommand();
  void setSubject(const QString &);
  void setBody(const QString &);
  void setAttachment(bool);

public:
  explicit MailForwardDialog(QWidget *parent = nullptr);
  void setOrderId(int orderId);
  int exec() override;
  int exec(int customerId, const QString &tpl);
};

#endif // MAILING_MAILFORWARDDIALOG_H
