// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ANTIQUACMD_H
#define ANTIQUACRM_ANTIQUACMD_H

#include <AGlobal>
#include <QCoreApplication>

class Settings;
class Provider;

class AntiquaCMD final : public QCoreApplication {
  Q_OBJECT

private:
  qint8 timeout = 15;
  Settings *m_cfg;
  int update(Provider *);

private Q_SLOTS:
  void queryAll();

public:
  explicit AntiquaCMD(int &argc, char **argv);
  int exec();
};

#endif // ANTIQUACRM_ANTIQUACMD_H
