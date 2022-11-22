// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SOCKETSERVER_H
#define ANTIQUACRM_SOCKETSERVER_H

#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>

class AntiquaSocketServer : public QLocalServer {
  Q_OBJECT

private:
  int timeout = 5000;
  QLocalSocket *m_listener;
  void createAction(const QJsonObject &obj);

protected:
  void incomingConnection(quintptr socketDescriptor);

Q_SIGNALS:
  void sendStatusMessage(const QString &message);
  void sendWindowOperation(const QJsonObject &obj);
  void sendPluginOperation(const QJsonObject &obj);

public:
  explicit AntiquaSocketServer(QObject *parent = nullptr);
  static const QString socketPath();
  ~AntiquaSocketServer();
};

#endif // ANTIQUACRM_SOCKETSERVER_H
