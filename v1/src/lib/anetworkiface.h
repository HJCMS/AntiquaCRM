// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_NETWORKIFACE_H
#define ANTIQUACRM_NETWORKIFACE_H

#include <QNetworkInterface>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief Testclass for Network Interfaces
 * @ingroup Network
 */
class ANetworkIface : public QNetworkInterface {
private:
  QStringList p_adresses;

public:
  explicit ANetworkIface();
  bool connectedIfaceExists();
  bool checkRemotePort(const QString &host, int port = 5432, int wait = 5);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKIFACE_H
