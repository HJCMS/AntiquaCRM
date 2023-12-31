// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmd.h"
#include "acmdproviders.h"

#include <QEventLoop>
#include <QPluginLoader>
#include <QStaticPlugin>
#include <QTimer>

AntiquaCMD::AntiquaCMD(int &argc, char **argv) : QCoreApplication{argc, argv} {
  setApplicationName("antiquacmd");
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
}

const QStringList AntiquaCMD::providers() {
  QStringList _l;
  const QStringList nFilter({"*.so"});
  QDir _dir(QCoreApplication::applicationDirPath() + "/providers");
  QDir::Filters sFilter(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
  foreach (QString p, _dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
    if (_dir.cd(p)) {
      foreach (QFileInfo info, _dir.entryInfoList(nFilter, sFilter)) {
        const QString _shared = info.filePath();
        if (QLibrary::isLibrary(_shared))
          _l.append(_shared);
      }
      _dir.cdUp();
    }
  }
  return _l;
}

int AntiquaCMD::update(ACmdProviders *provider) {
  QTimer timer;
  QEventLoop loop;
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(provider, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(provider, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (!provider->init())
    return 1;

  provider->start();
  timer.start((timeout * 1000));
  if (loop.exec() == 0) {
    return 0;
  }

  return 1;
}

void AntiquaCMD::queryAll() {
  const QStringList _plugins = providers();
  QPluginLoader _loader;
  for (int i = 0; i < _plugins.size(); i++) {
    _loader.setFileName(_plugins.at(i));
    QJsonObject _pinfo = _loader.metaData().value("MetaData").toObject();
    QObject *obj = _loader.instance();
    if (obj == nullptr) {
      qWarning("Can not load '%s' plugin!",
               qPrintable(_pinfo.value("Name").toString()));
      continue;
    }
    ACmdProviders *prv = qobject_cast<ACmdProviders *>(obj);
    if (prv == nullptr)
      continue;

    if (update(prv) == 0)
      prv->deleteLater();
  }
}

int AntiquaCMD::exec() {
  queryAll();
  return 0;
}
