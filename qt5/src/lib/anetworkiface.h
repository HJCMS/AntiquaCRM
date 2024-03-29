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
 * @brief Testclass for Network Interfaces
 * @ingroup Network
 */
class ANTIQUACRM_LIBRARY ANetworkIface : public QNetworkInterface {
private:
  QStringList p_adresses;

public:
  explicit ANetworkIface();
  bool connectedIfaceExists();
  bool checkRemotePort(const QString &host, int port = 5432, int wait = 5);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKIFACE_H
