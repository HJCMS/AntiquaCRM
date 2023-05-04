// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_ABEBOOKS_H
#define ANTIQUACMD_ABEBOOKS_H

#include <QDomDocument>
#include <QObject>

#ifndef ANTIQUACMD_PROVIDER_H
#include "provider.h"
#endif

class AbeBooksDocument;

class AbeBooks final : public Provider {
  Q_OBJECT

private:
  /**
   * @brief Codec for XML/SOAP Request
   * @warning AbeBooks using IS0-8859-1 with invalid Content-Header
   */
  QTextCodec *m_abeCodec;

  void initConfiguration() override;

  AbeBooksDocument initDocument();

  const QString provider() const override { return QString("AbeBooks"); };

  const QUrl apiQuery(const QString &) override;

  void prepareContent(const QJsonDocument &) override{
      /* AbeBooks only using XML/SOAP */
  };

  void prepareContent(const QDomDocument &);

private Q_SLOTS:
  void incomingCodec(QTextCodec *);
  void responsed(const QByteArray &) override;

public Q_SLOTS:
  void start() override;

public:
  explicit AbeBooks(QObject *parent = nullptr);
  bool init() override;
};

#endif
