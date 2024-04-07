// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_NETWORKIFACE_H
#define ANTIQUACRM_NETWORKIFACE_H

#include <QNetworkInterface>
#include <QStringList>
#include <AGlobal>

namespace AntiquaCRM {

/**
 * @class ANetworkIface
 * @brief Network Interfaces testings
 *
 * @ingroup Network
 */
class ANTIQUACRM_LIBRARY ANetworkIface : public QNetworkInterface {
private:
  int countInterfaces = 0;

public:
  explicit ANetworkIface();
  ~ANetworkIface();

  /**
   * @brief Simple check if connected interfaces exists
   */
  bool connectedIfaceExists();

  /**
   * @brief Checks a specified remote port.
   * @param host - hostname (FQDH)
   * @param port - port
   * @param wait - timeout
   */
  bool checkRemotePort(const QString &host, int port = 5432, int wait = 5);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKIFACE_H
