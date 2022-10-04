// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PRELOADER
#define ANTIQUA_PRELOADER

#include <QObject>
#include <QThreadPool>
#include <AntiquaCRM>

class AntiquaPreloader : public QThreadPool {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;

Q_SIGNALS:
  void statusMessage(const QString &);

public:
  explicit AntiquaPreloader(QObject *parent = nullptr);
  const QList<QRunnable*> getThreads();
};

#endif
