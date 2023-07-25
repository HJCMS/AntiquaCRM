// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_MAIL_DIALOG_H
#define ANTIQUACRM_WIDGETS_MAIL_DIALOG_H

#include <AntiquaWidgets>
#include <QCloseEvent>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>

namespace AntiquaCRM {

class MailTemplateKeys;
class MailAttachments;

/**
 * @class MailDialog
 * @brief The Mail Dialog class
 *
 * Create/Edit/Forward eMail to System Mailler application.
 *
 * https://docs.microsoft.com/de-de/appcenter/cli/
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY MailDialog final : public QDialog {
  Q_OBJECT

private:
  qint64 p_customerId = -1;
  qint64 p_orderId = -1;
  int p_gender = -1;
  QString p_tb_caller = QString();

  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  AntiquaCRM::MailTemplateKeys *m_keys;
  AntiquaCRM::MailAttachments *m_attachments;

  QJsonObject templateData;
  QRegularExpression p_regexp = QRegularExpression("(@[A-Z_]+@)+");

  AntiquaCRM::ALabel *m_subject;
  AntiquaCRM::TextField *m_body;

  QDialogButtonBox *m_btnBox;
  QPushButton *btn_close;
  QPushButton *btn_email;

  QStatusBar *m_statusBar;

  void closeEvent(QCloseEvent *) override;

  bool selectTemplate(const QString &caller);

  const QJsonObject articleObject(const QVariant &value);

private Q_SLOTS:
  void setRecipientData();
  void setFailed();
  void setMailCommand();
  void setSubject(const QString &txt);
  void setBody(const QString &txt);
  void setAttachment();

public:
  /**
   * @param parent - parent object
   */
  explicit MailDialog(QWidget *parent = nullptr);

  /**
   * @brief prevent dialog without options
   * @return QDialog::Rejected
   */
  int exec() override final;

  /**
   * @brief execute with options
   * @param options - Json Object
   *
   * Sample code and description below:
   * @code
   *  QJsonObject _obj;
   *  _obj.insert("tb_tab", "ORDERS");
   *  _obj.insert("tb_caller", "MAIL_ACTION_SHIPPING_NOTICE");
   *  _obj.insert("CRM_CUSTOMER_ID", 1);
   *  _obj.insert("CRM_ORDER_ID", 1);
   *  _obj.insert("CRM_INVOICE_ID", 1);
   * @endcode
   *
   * The "tb_tab" variable is used to create relationships to the destination.
   *
   * The options depend on the variable "tb_caller" from the database table
   * "ui_template_body".
   *
   * All other parameters must be able to be resolved in the
   * "ui_template_keys.tk_key" or "antiquacrm_company.ac_class" table.
   *
   * @return QDialog::DialogCode
   */
  int exec(const QJsonObject &data);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_MAIL_DIALOG_H
