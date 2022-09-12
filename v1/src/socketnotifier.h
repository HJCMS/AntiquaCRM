// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SOCKETNOTIFIER_H
#define ANTIQUACRM_SOCKETNOTIFIER_H

#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QString>

class SocketNotifier : public QLocalServer {
  Q_OBJECT

private:
  int timeout = 5000;
  QLocalSocket *m_listener;

protected:
  void incomingConnection(quintptr socketDescriptor);

public:
  explicit SocketNotifier(QObject *parent = nullptr);
  static const QString name();
};

#endif // ANTIQUACRM_SOCKETNOTIFIER_H
