// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WHSOFT_PLUGIN_H
#define ANTIQUA_WHSOFT_PLUGIN_H

#include <QObject>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>

class ANTIQUACORE_EXPORT WHSoft : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.Antiqua.WHSoft" FILE "whsoft.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_whsoft;

public:
  bool createInterface(QObject *parent);

  inline const QString configGroupName() { return QString("whsoft"); };

  const QUrl apiQuery(const QString &apiKey, const QString &operation);

  /**
   * @brief Übersetzt die Buchfreund.de Json Datenfelder zu SQL Spaltenname.
   * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
   * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
   * übersetzt werden.
   */
  const QMap<QString, QString> fieldTranslate();
  /**
   * @brief SQL Datenfeld zurück geben
   * @param key - Json Parameter
   * @return SQL Column | QString()
   */
  const QString sqlParam(const QString &key);

  /**
   * @brief Such mit SQL Feldname nach API Parameter
   * @param key - SQL Tabellen Feldname
   * @return Json Parameter | QString()
   */
  const QString apiParam(const QString &key);
};

#endif // ANTIQUA_WHSOFT_PLUGIN_H
