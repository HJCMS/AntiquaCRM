// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BUCHFREUND_PLUGIN_H
#define BUCHFREUND_PLUGIN_H

#include <QJsonDocument>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTextEdit>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

/**
 * @brief Hauptseite des Dienstleisters
 * @ingroup Whsoft Plugin
 */
class ANTIQUACORE_EXPORT Buchfreund final : public Antiqua::ProviderWidget {
  Q_OBJECT

private:
  QLineEdit *m_articleId;
  QSpinBox *m_count;
  QTextEdit *m_response;

  void jsonQuery(const QString &operation, const QJsonDocument &doc);

private Q_SLOTS:
  void updateArticleCount();
  void queryResponse(const QJsonDocument &);

public:
  explicit Buchfreund(const QString &widgetId, QWidget *parent = nullptr);
};

#endif // BUCHFREUND_PLUGIN_H
