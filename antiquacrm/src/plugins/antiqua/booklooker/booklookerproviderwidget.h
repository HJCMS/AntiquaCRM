// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERPROVIDERWIDGET_PLUGIN_H
#define BOOKLOOKERPROVIDERWIDGET_PLUGIN_H

#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class ANTIQUACORE_EXPORT BooklookerProviderWidget final
    : public Antiqua::ProviderWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit BooklookerProviderWidget(const QString &widgetId,
                                  QWidget *parent = nullptr);
};

#endif // BOOKLOOKERPROVIDERWIDGET_PLUGIN_H
