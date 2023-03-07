// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_AUTHENTICATOR_H
#define BOOKLOOKER_AUTHENTICATOR_H

#include <AntiquaCRM>
#include <AntiquaCRMPlugin>
#include <QObject>

/**
 * @brief The BooklookerAuthenticator class
 * @ingroup BooklookerPlugin
 */
class ANTIQUACRM_LIBRARY BooklookerAuthenticator : public QObject {
  Q_OBJECT

public:
  explicit BooklookerAuthenticator(QObject *parent = nullptr);
};

#endif // BOOKLOOKER_AUTHENTICATOR_H
