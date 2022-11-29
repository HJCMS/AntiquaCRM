// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILING_MAILATTACHMENTS_H
#define MAILING_MAILATTACHMENTS_H

#include <QFileInfo>
#include <QFrame>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <ASettings>

class AttachmentInfo final : public QLineEdit {
  Q_OBJECT

private:
  QString p_tip;
  QString p_path;

public Q_SLOTS:
  void restore();
  void setPath(const QString &path);

public:
  explicit AttachmentInfo(QWidget *parent = nullptr);
  const QString path();
};

class MailAttachments final : public QFrame {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AttachmentInfo *m_info;
  QPushButton *m_btn;

private Q_SLOTS:
  void openFileDialog();

public Q_SLOTS:
  void setActive(bool);
  void clear() { m_info->restore(); }

public:
  explicit MailAttachments(QWidget *parent = nullptr);
  bool setAttachment(const QString &name);
  bool exists();
  const QFileInfo attachment();
};

#endif // MAILING_MAILATTACHMENTS_H
