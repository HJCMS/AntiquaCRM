// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILATTACHMENTS_UTILS_H
#define MAILATTACHMENTS_UTILS_H

#include <QFileInfo>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class ApplSettings;

class AttachmentInfo final : public QLabel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

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
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *m_cfg;
  AttachmentInfo *m_info;
  QPushButton *m_btn;

private Q_SLOTS:
  void openFileDialog();

public Q_SLOTS:
  void setAttachment(const QString &name);
  void clear() { m_info->restore(); }

public:
  explicit MailAttachments(QWidget *parent = nullptr);
  bool exists();
  const QFileInfo attachment();
};

#endif // MAILATTACHMENTS_UTILS_H
