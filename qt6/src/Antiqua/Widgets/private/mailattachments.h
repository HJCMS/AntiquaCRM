// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_MAILATTACHMENTS_H
#define ANTIQUACRM_PRIVATE_MAILATTACHMENTS_H

#include <AntiquaWidgets>
#include <QFileInfo>
#include <QFrame>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY AttachmentInfo final : public QLineEdit {
  Q_OBJECT

private:
  QString p_tip;
  QString p_path;

  const QString styleInvalid() const;
  const QString styleValid() const;

public Q_SLOTS:
  void restore();
  void setExistsFrame(bool);
  void setPath(const QString &path);

public:
  explicit AttachmentInfo(QWidget *parent = nullptr);
  const QString path();
};

class ANTIQUACRM_LIBRARY MailAttachments final : public QFrame {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::AttachmentInfo *m_info;
  QPushButton *m_btn;

  const QDir invoiceArchive();

private Q_SLOTS:
  void openFileDialog();

Q_SIGNALS:
  void sendAttachmentChanged();

public Q_SLOTS:
  void setActive(bool);
  void clear() { m_info->restore(); }

public:
  explicit MailAttachments(QWidget *parent = nullptr);
  bool setAttachment(const QString &name);
  bool exists();
  const QFileInfo attachment();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_MAILATTACHMENTS_H
