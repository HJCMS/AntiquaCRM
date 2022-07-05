// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BUCHFREUND_PLUGIN_H
#define BUCHFREUND_PLUGIN_H

#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief Hauptseite des Dienstleisters
 * @ingroup Whsoft Plugin
 */
class ANTIQUACORE_EXPORT Buchfreund final : public Antiqua::ProviderWidget {
  Q_OBJECT

public:
  explicit Buchfreund(const QString &widgetId, QWidget *parent = nullptr);
};

#endif // BUCHFREUND_PLUGIN_H
