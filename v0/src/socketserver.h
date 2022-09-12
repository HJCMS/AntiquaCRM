// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QString>
#include <QLocalServer>
#include <QLocalSocket>

class SocketServer : public QLocalServer {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int timeout = 5000;
  QLocalSocket *m_listener;

  void readClientConnection();

protected:
  void incomingConnection(quintptr socketDescriptor);

Q_SIGNALS:
  void showWindow();
  void statusMessage(const QString &);

public:
  explicit SocketServer(QObject *parent = nullptr);
  static const QString name();
};

#endif // SOCKETSERVER_H
