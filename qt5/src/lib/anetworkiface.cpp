// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworkiface.h"

#include <QDebug>
#include <QHostAddress>
#include <QNetworkAddressEntry>
#include <QTcpSocket>

namespace AntiquaCRM {

ANetworkIface::ANetworkIface() : QNetworkInterface{} {
  p_adresses.clear();
  foreach (QNetworkInterface net, QNetworkInterface::allInterfaces()) {
    if (net.flags() & !(QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
      continue;

    if (net.flags() & QNetworkInterface::IsLoopBack)
      continue;

    if (net.type() & (QNetworkInterface::Ethernet | QNetworkInterface::Wifi)) {
      foreach (QNetworkAddressEntry a, net.addressEntries()) {
        if (a.ip().isGlobal() && !a.ip().isLinkLocal()) {
          p_adresses.append(a.ip().toString());
        }
      }
    }
  }
}

bool ANetworkIface::connectedIfaceExists() { return (p_adresses.count() > 0); }

bool ANetworkIface::checkRemotePort(const QString &host, int port, int wait) {
  bool b = false;
  QTcpSocket sock;
  sock.connectToHost(host, port, QIODevice::ReadOnly);
  if (sock.waitForConnected((wait * 1000))) {
    b = true;
  } else {
    switch (sock.error()) {
    case (QAbstractSocket::HostNotFoundError):
      qWarning("CheckPort: Host not found error!");
      break;

    case (QAbstractSocket::RemoteHostClosedError):
      qWarning("CheckPort: Host request rejected!");
      break;

    default:
      qWarning("Unknown Socker error!");
      break;
    }
  }
  sock.close();
  return b;
}

}; // namespace AntiquaCRM
