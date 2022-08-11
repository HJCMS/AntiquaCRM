// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILFORWARDDIALOG_UTILS_H
#define MAILFORWARDDIALOG_UTILS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QRegularExpression>
#include <QStatusBar>
#include <QWidget>

#include "messageselecter.h"
#include <AntiquaCRM>
#include <SqlCore>

class MailBody;
class MailTemplateKeys;

/**
 * @brief E-Mail Nachrichten für CLI erstellen.
 * https://docs.microsoft.com/de-de/appcenter/cli/
 */
class MailForwardDialog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_customerId = -1;
  int p_orderId = -1;
  int p_gender = -1;
  ApplSettings *m_cfg;
  HJCMS::SqlCore *m_sql;
  MailTemplateKeys *m_keys;
  MessageSelecter *m_selecter;
  MailBody *m_mailBody;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;
  QRegularExpression p_expression = QRegularExpression("(@[A-Z_]+@)+");

  bool createMailSelect(const QString &tpl);
  bool queryDataFields(int customerId);

private Q_SLOTS:
  void setSubject(const QString &subject);
  void setBody(const QString &body);

public:
  explicit MailForwardDialog(QWidget *parent = nullptr);
  void setOrderId(int orderId);
  int exec(int customerId, const QString &tpl);
};

#endif // MAILFORWARDDIALOG_UTILS_H
