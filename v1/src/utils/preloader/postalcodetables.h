// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_POSTALCODETABLES_H
#define ANTIQUA_POSTALCODETABLES_H

#include <AntiquaCRM>
#include <QJsonArray>
#include <QList>
#include <QPair>
#include <QRunnable>

class PostalcodeTables final : public QRunnable {
private:
  AntiquaCRM::ASqlCore *m_sql;
  const QList<QPair<QString, QString>> postalCodeTables();
  const QJsonArray createPostalCodes(const QString &query);

public:
  explicit PostalcodeTables(AntiquaCRM::ASqlCore *sql);
  static const QString name() { return QString("Postcalcodes"); };
  void run() override;
};

#endif // ANTIQUA_POSTALCODETABLES_H
