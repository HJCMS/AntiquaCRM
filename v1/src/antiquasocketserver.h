// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SOCKETSERVER_H
#define ANTIQUACRM_SOCKETSERVER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>

class AntiquaSocketServer : public QLocalServer {
  Q_OBJECT

private:
  int timeout = 5000;
  QLocalSocket *m_listener;

protected:
  void incomingConnection(quintptr socketDescriptor);

public:
  explicit AntiquaSocketServer(QObject *parent = nullptr);
  static const QString name();
};

#endif // ANTIQUACRM_SOCKETSERVER_H