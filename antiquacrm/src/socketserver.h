// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtNetwork/QLocalServer>

class SocketServer : public QLocalServer {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int timeout = 5000;

protected:
  void incomingConnection(quintptr socketDescriptor);

public:
  explicit SocketServer(QObject *parent = nullptr);
  static const QString name();
};

#endif // SOCKETSERVER_H
