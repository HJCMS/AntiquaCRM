// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#ifndef ANTIQUA_INTERFACE_H
#define ANTIQUA_INTERFACE_H

#include <QMap>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>
#include <QUrl>
#include <QScrollArea>
#include <QJsonDocument>

#include <AntiquaCRM>
#ifndef ANTIQUA_INTERFACE
# define ANTIQUA_INTERFACE "de.hjcms.@PROJECTNAME@.Antiqua"
#endif

namespace Antiqua {

  class ANTIQUACORE_EXPORT InterfaceWidget : public QScrollArea
  {
    Q_OBJECT

  Q_SIGNALS:
    void openCustomer(int);

  public Q_SLOTS:
    virtual void setContent(const QJsonDocument &) = 0;
    Q_INVOKABLE virtual void checkCustomer() = 0;

  public:
    explicit InterfaceWidget(const QString &widgetId, QWidget *parent = nullptr) : QScrollArea{parent}  {
      setObjectName(widgetId);
    };
    virtual const QList<int> getArticleIds() = 0;
  };

  class ANTIQUACORE_EXPORT Interface : public QObject
  {
    Q_OBJECT
    Q_CLASSINFO("Interface", ANTIQUA_INTERFACE)
    Q_CLASSINFO("Description", "@DISPLAYNAME@ Interface")
    Q_CLASSINFO("Version", "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@")
    Q_CLASSINFO("Url", "https://www.hjcms.de")

  Q_SIGNALS:
    void listResponse(const QJsonDocument &);

  protected Q_SLOTS:
    virtual void prepareJsonListResponse(const QJsonDocument &) = 0;

  public:
    virtual bool createInterface(QObject * parent) = 0;

    /**
     * @brief Tab das eingefügt wird wenn Daten empfangen werden!
     * Die einzelnen Tabs werden über den Objektnamen Identifiziert und sind deshalb Unique.
     * @note widgetId muss Identisch mit "objectName" und "tabTitle" sein!
     */
    virtual Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget * parent) = 0;

    /**
     * @brief Wird für Konfigurationen benötigt
     */
    virtual const QString configGroupName() const = 0;

    /**
     * @brief Übersetzt die Json Datenfelder zu SQL Spaltenname.
     * Wir verwenden andere Datenfeld Bezeichnungen als der Dienstanbieter.
     * Deshalb müssen die Parameter vor der Weiterleitung an dieser Stelle
     * übersetzt werden.
     */
    virtual const QMap<QString, QString> fieldTranslate() const = 0;

    /**
     * @brief SQL Datenfeld zurück geben
     * @param key - Json Parameter
     * @return SQL Column | QString()
     */
    virtual const QString sqlParam(const QString &key) = 0;

    /**
     * @brief Suche mit SQL Feldname nach API Parameter
     * @param key - SQL Tabellen Feldname
     * @return Json Parameter | QString()
     */
    virtual const QString apiParam(const QString &key) = 0;

    /**
     * @brief Abfrage ob Daten vorhanden sind.
     */
    virtual void queryMenueEntries() = 0;
  };
};

Q_DECLARE_INTERFACE(Antiqua::Interface, ANTIQUA_INTERFACE)

#endif // ANTIQUA_INTERFACE_H
