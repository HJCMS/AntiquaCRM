// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_ANTIQUACACHEFILES
#define ANTIQUA_ANTIQUACACHEFILES

#include <QObject>
#include <QString>
#include <QThread>

#include "workload.h"

class AntiquaCacheFiles : public QObject {
  Q_OBJECT

private:
  QThread p_thread;
  const QList<Workload*> getTasks();

Q_SIGNALS:
  void statusMessage(const QString &);

public:
  explicit AntiquaCacheFiles(QObject *parent = nullptr);
  void createCaches();
};

#endif // ANTIQUA_ANTIQUACACHEFILES