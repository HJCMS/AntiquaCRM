// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_ABEBOOKS_PLUGIN_H
#define ANTIQUA_ABEBOOKS_PLUGIN_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>
#include <Networking>

class ANTIQUACORE_EXPORT AbeBooksQuery final : public QObject {
  Q_OBJECT

private:
  const QUrl apiQuery(const QString &operation);

Q_SIGNALS:
  void listResponsed(const QJsonDocument &doc);
  void orderResponsed(const QJsonDocument &doc);

public Q_SLOTS:
  void queryList();
  void queryOrder(const QString &bfId);
  void customQuery(const QString &operation, const QJsonDocument &doc);

public:
  explicit AbeBooksQuery(QObject *parent = nullptr);
};

class ANTIQUACORE_EXPORT AbeBooksPurchaser final
    : public Antiqua::PurchaserWidget {
  Q_OBJECT

public Q_SLOTS:
  void setCustomerId(int customerId);
  void setValue(const QString &objName, const QVariant &value);

public:
  QTextEdit *rechnungsadresse;
  QTextEdit *lieferadresse;
  explicit AbeBooksPurchaser(QWidget *parent = nullptr);
  const QVariant getValue(const QString &objName);
};

class ANTIQUACORE_EXPORT AbeBooksProviderWidget final
    : public Antiqua::ProviderWidget {
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
  explicit AbeBooksProviderWidget(const QString &widgetId,
                                  QWidget *parent = nullptr);
};

class ANTIQUACORE_EXPORT AbeBooksIfaceWidget final
    : public Antiqua::InterfaceWidget {
  Q_OBJECT

private:
  Antiqua::PurchaserOrderTable *m_orderTable;
  AbeBooksPurchaser *m_purchaserWidget;
  QJsonDocument p_currentDocument;

  const QVariant tableData(int row, int column);
  const QJsonDocument customerRequest(const QJsonObject &object);
  void parseAddressBody(const QString &section, const QJsonObject &object);

private Q_SLOTS:
  void readCurrentArticleIds();

public Q_SLOTS:
  void createCustomerDocument();
  void setContent(const QJsonDocument &);
  void createOrderRequest(const QString &bfId);

public:
  AbeBooksIfaceWidget(const QString &widgetId, QWidget *parent = nullptr);
  void setCustomerId(int customerId);
  const QMap<QString, QString> fieldTranslate() const;
  const ProviderOrder getProviderOrder();
};

class ANTIQUACORE_EXPORT AbeBooks : public Antiqua::Interface {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "de.hjcms.antiquacrm.AntiquaFactoryInterface" FILE
                        "abebooks.json")
  Q_INTERFACES(Antiqua::Interface)

private:
  QObject *m_abeBooks;

  /**
   * @brief AbeBooksIfaceWidget
   * Antiqua::InterfaceWidget
   */
  AbeBooksIfaceWidget *m_abeBooksIfaceWidget;

  /**
   * @brief AbeBooksProviderWidget
   * Antiqua::ProviderWidget
   */
  AbeBooksProviderWidget *m_abeBooksProviderWidget;

private Q_SLOTS:
  void prepareJsonListResponse(const QJsonDocument &);
  void responseAnswerCheck(const QJsonDocument &);

public:
  bool createInterface(QObject *parent);

  /**
   * @brief Hauptseite
   */
  Antiqua::ProviderWidget *providerWidget(const QString &widgetId,
                                          QWidget *parent);

  /**
   * @brief Bestellungen
   */
  Antiqua::InterfaceWidget *addWidget(const QString &widgetId, QWidget *parent);

  const QString provider() const;

  /**
   * @brief Wird für Anwender Konfiguration benötigt
   */
  const QString configGroup() const;

  /**
   * @brief Menü Einträge suchen
   */
  void queryMenueEntries();

  /**
   * @brief Artikelbestand ändern!
   * @param articleId
   * @param count
   */
  void updateArticleCount(int articleId, int count);
};

#endif // ANTIQUA_ABEBOOKS_PLUGIN_H
