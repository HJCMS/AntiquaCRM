// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSPROVIDERWIDGET_PLUGIN_H
#define ABEBOOKSPROVIDERWIDGET_PLUGIN_H

#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class ANTIQUACORE_EXPORT AbeBooksProviderWidget final
    : public Antiqua::ProviderWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit AbeBooksProviderWidget(const QString &widgetId,
                                  QWidget *parent = nullptr);
};

#endif // ABEBOOKSPROVIDERWIDGET_PLUGIN_H
