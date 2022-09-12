// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef APPLICATIONCLIENT_UTILS_H
#define APPLICATIONCLIENT_UTILS_H

#include <QLocalSocket>
#include <QObject>

class ApplicationClient final : public QLocalSocket {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool isConnected = false;
  int p_timeout = 1500;
  QString p_message_buffer;

  void initConnection();
  void writeToServer();

private Q_SLOTS:
  void getErrors(QLocalSocket::LocalSocketError error);
  void getState(QLocalSocket::LocalSocketState state);

public Q_SLOTS:
  void sendMessage(const QString &);

public:
  explicit ApplicationClient(QObject *parent = nullptr);
  virtual ~ApplicationClient();
};

#endif // APPLICATIONCLIENT_UTILS_H
