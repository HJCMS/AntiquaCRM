// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_NETWORKIFACE_H
#define ANTIQUACRM_NETWORKIFACE_H

#include <QHostAddress>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>

namespace AntiquaCRM {

class ANetworkIface : public QNetworkInterface {
private:
  QStringList p_adresses;

public:
  explicit ANetworkIface();
  bool connectedIfaceExists();
  bool checkSqlPort(const QString &host, int port = 5432, int wait = 5);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKIFACE_H
