// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_BOOKLOOKER_H
#define ANTIQUACMD_BOOKLOOKER_H

#include <QDate>
#include <QJsonValue>
#include <QObject>
#include <QMutex>

#ifndef ANTIQUACMD_PROVIDER_H
#include "provider.h"
#endif

class BookLooker final : public Provider {
  Q_OBJECT

private:
  void initConfiguration() override;

  const QString provider() const override { return QString("Booklooker"); };

  const QUrl apiQuery(const QString &) override;

  const QString dateString(const QDate &date = QDate::currentDate()) const;

  void prepareContent(const QJsonDocument &) override;

  void setTokenCookie(const QString &token);

  bool isCookieExpired();

  void authenticate();

  void query();

private Q_SLOTS:
  void responsed(const QByteArray &) override;

public Q_SLOTS:
  void start() override;

public:
  explicit BookLooker(QObject *parent = nullptr);
  bool init() override;
};

#endif
