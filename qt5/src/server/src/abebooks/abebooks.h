// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABEBOOKS_H
#define ANTIQUACRM_ABEBOOKS_H

#include "provider.h"

#include <QDomDocument>
#include <QObject>

class AbeBooksDocument;

class AbeBooks final : public Provider {
  Q_OBJECT

private:
  void initConfiguration() override;

  AbeBooksDocument initDocument();

  const QString provider() const override { return QString("AbeBooks"); };

  const QUrl apiQuery(const QString &) override;

  void prepareContent(const QJsonDocument &) override{
      /* AbeBooks verwendet XML */
  };

  void prepareContent(const QDomDocument &);

private Q_SLOTS:
  void responsed(const QByteArray &) override;

public Q_SLOTS:
  void start() override;

public:
  explicit AbeBooks(QObject *parent = nullptr);
  bool init() override;
};

#endif
