// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_ANTIQUACMD_H
#define ANTIQUACMD_ANTIQUACMD_H

#include <AGlobal>
#include <ASettings>
#include <QMutex>
#include <QCoreApplication>

class ACmdProviders;

class AntiquaCMD final : public QCoreApplication {
  Q_OBJECT

private:
  qint8 timeout = 15;
  const QStringList providers();
  int update(ACmdProviders *);
  void queryAll();

public:
  explicit AntiquaCMD(int &argc, char **argv);
  int exec();
};

#endif // ANTIQUACMD_ANTIQUACMD_H
