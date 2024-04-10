// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworkiface.h"

#include <QDebug>
#include <QHostAddress>
#include <QNetworkAddressEntry>
#include <QTcpSocket>

namespace AntiquaCRM
{

ANetworkIface::ANetworkIface() : QNetworkInterface{} {
  foreach (QNetworkInterface net, QNetworkInterface::allInterfaces()) {
    if (net.flags() & !(QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
      continue;

    if (net.flags() & QNetworkInterface::IsLoopBack)
      continue;

    if (net.type() & (QNetworkInterface::Ethernet | QNetworkInterface::Wifi)) {
      QStringList p_list;
      foreach (QNetworkAddressEntry a, net.addressEntries()) {
        if (a.ip().isGlobal() && !a.ip().isLinkLocal()) {
          p_list.append(a.ip().toString());
        }
      }

      if (p_list.size() > 0)
        countInterfaces++;

      p_list.clear();
    }
  }
}

ANetworkIface::~ANetworkIface() {
}

bool ANetworkIface::connectedIfaceExists() {
  return (countInterfaces > 0);
}

bool ANetworkIface::isLoopbackInterface(const QString& iface) {
  QStringList p_list({"localhost"});
  foreach (QNetworkInterface net, QNetworkInterface::allInterfaces()) {
    if (net.flags() & !(QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
      continue;

    if (net.flags() & QNetworkInterface::IsLoopBack) {
      foreach (QNetworkAddressEntry a, net.addressEntries()) {
        p_list.append(a.ip().toString());
      }
    }
  }
  return (p_list.contains(iface));
}

bool ANetworkIface::checkRemotePort(const QString& host, int port, int wait) {
  bool _b = false;
  if (!connectedIfaceExists())
    return _b;

  QTcpSocket _s;
  _s.connectToHost(host, port, QIODevice::ReadOnly);
  if (_s.waitForConnected((wait * 1000))) {
    _b = true;
  }
  _s.close();

  return _b;
}

}; // namespace AntiquaCRM
