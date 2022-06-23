// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSQUERY_PLUGIN_H
#define ABEBOOKSQUERY_PLUGIN_H

#include <QDomDocument>
#include <QDomElement>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QUrl>

#include <AntiquaCRM>
#include <AntiquaInterface>

#include "abebooksdocument.h"

class ANTIQUACORE_EXPORT AbeBooksRequester final : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *config;
  QNetworkAccessManager *m_networker;

  const QNetworkRequest initRequest();

  AbeBooksDocument createDocument();

  /**
   * @brief AbeBooks using ISO-8859-1
   */
  const QString toISO88591(const QString &key);

public:
  explicit AbeBooksRequester(QObject *parent = nullptr);
};

#endif // ABEBOOKSQUERY_PLUGIN_H
