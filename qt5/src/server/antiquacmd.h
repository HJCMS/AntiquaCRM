// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_ANTIQUACMD_H
#define ANTIQUACMD_ANTIQUACMD_H

#include <AGlobal>
#include <ASettings>
#include <QCoreApplication>

class Provider;

class AntiquaCMD final : public QCoreApplication {
  Q_OBJECT

private:
  qint8 timeout = 15;
  AntiquaCRM::ASettings *m_cfg;
  int update(Provider *);

private Q_SLOTS:
  void queryAll();

public:
  explicit AntiquaCMD(int &argc, char **argv);
  int exec();
};

#endif // ANTIQUACMD_ANTIQUACMD_H
