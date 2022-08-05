// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGESELECTER_UTILS_H
#define MESSAGESELECTER_UTILS_H

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

class MessageCaller final {
private:
  QString p_caller;
  QString p_title;
  QString p_body;

public:
  explicit MessageCaller();
  void setCaller(const QString &);
  const QString getCaller();
  void setTitle(const QString &);
  const QString getTitle();
  void setBody(const QString &);
  const QString getBody();
};

class MessageSelecter final : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_selecter;
  QLabel *m_title;
  QLabel *m_caller;
  QMap<int,MessageCaller> p_map;

private Q_SLOTS:
  void setContent(int);

Q_SIGNALS:
  void sendBody(const QString &);

public:
  explicit MessageSelecter(QWidget *parent = nullptr);
  void setSelecters(const QList<MessageCaller> &list);
  const QList<MessageCaller> getSelecters();
  const QString getCaller();
  const QString getTitle();
};

#endif // MESSAGESELECTER_UTILS_H
