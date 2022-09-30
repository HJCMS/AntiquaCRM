// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERUPDATE_TASK_H
#define INVENTORY_PROVIDERUPDATE_TASK_H

#include <AntiquaInterface>
#include <QRunnable>

class ProviderUpdateTask : public QRunnable {
private:
  Antiqua::Interface *m_worker = nullptr;

public:
  explicit ProviderUpdateTask();
  bool setHandle(Antiqua::Interface *handle);
  void run();
};

#endif // INVENTORY_PROVIDERUPDATE_TASK_H
