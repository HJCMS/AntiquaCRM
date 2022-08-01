// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PURCHASEOVERVIEW_H
#define ANTIQUA_PURCHASEOVERVIEW_H

#include <QDateTime>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QTabWidget>
#include <QMap>
#include <QLineEdit>
#include <QMetaObject>
#include <QMetaType>
#include <QObject>
#include <QTextEdit>
#include <QToolBar>
#include <QPushButton>
#include <QUrl>
#include <QWidget>

#include "antiqua_global.h"

/**
 * @ingroup Antiqua Plugin Interface
 */
namespace Antiqua {

  class PurchasePaymentInfo;
  class PurchaserOrderTable;

#ifdef ANTIQUA_DEVELOPEMENT
  class PurchaseDebugTable;
#endif

/**
 * @class Antiqua::PurchaseOverview
 * @ingroup Antiqua Plugin Interface
 * @brief Bestellübersicht
 */
class ANTIQUACORE_EXPORT PurchaseOverview final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int c_id = 0;
  QToolBar *m_toolbar;
  QPushButton *btn_checkArticle;

  /**
   * @brief c_id
   */
  QLineEdit *m_customerId;

  /**
   * @brief Full Purchaser Name
   */
  QLabel *m_customerInfo;

  /**
   * @brief Tab Section
   * @{
   */
  QTabWidget *m_tabWidget;

  /**
   * @brief c_postal_address
   */
  QTextEdit *m_billingAddress;

  /**
   * @brief c_shipping_address
   */
  QTextEdit *m_shippingAddress;

  /**
   * @brief Käufer Anmerkungen und/oder Fragen
   */
  QTextEdit *m_comments;

#ifdef ANTIQUA_DEVELOPEMENT
  /**
   * @brief Developement Summary TabelView
   */
  PurchaseDebugTable *m_debugTable;
#endif

  QMap<QString, QVariant> m_purchaseData;

  /**< @} */

  /**
   * @brief Article Orders List
   * @see Antiqua::PurchaserOrderTable
   */
  Antiqua::PurchaserOrderTable *m_table;

  /**
   * @brief Zahlungs Informationen
   */
  Antiqua::PurchasePaymentInfo *m_paymentInfo;

private Q_SLOTS:
  void customerChanged(const QString &);

public Q_SLOTS:
  /**
   * @brief Kundennummer einfügen
   */
  void setCustomerId(int id);

Q_SIGNALS:
  void sendUpdateOrderStatus(Antiqua::PaymentStatus);
  void inspectArticle(int);
  void customerIdChanged(int);
  void checkOrders();
  void checkCustomer();

public:
  explicit PurchaseOverview(const QString &id, QWidget *parent = nullptr);

  int getCustomerId();

  /**
   * @brief Suchfelder für die Kundenabfrage
   * Die Kundenabfrage erfolgt über diese Tabellenfelder!
   * @code
   *  {"c_firstname","c_lastname","c_postalcode","c_location"}
   * @endcode
   */
  const QStringList customerSearchFields() const;

  /**
   * @brief Alle internen Artikel Nummern aus der Bestelltabelle ziehen.
   * @note Das sind nicht die Artikel Nummern des Dienstleisters!
   */
  const QList<int> getArticleIDs();

  /**
   * @brief Setze ComboBox Daten für Diensteleister Bestellungsupdate Aktionen.
   */
  void setOrderUpdateActions(const QMap<Antiqua::PaymentStatus, QString> &);

  /**
   * @brief In Bestelltabelle eine reihe hinzufügen
   */
  void setTableCount(int count);

  /**
   * @brief Anzahl der Bestellartikel
   */
  int getTableCount();

  /**
   * @brief Eine Zelle in reihe Ändern/Einfügen
   */
  bool setTableData(int row, int column, const QVariant &value);

  /**
   * @brief Nehme Datensatz von Reihe/Zelle
   */
  const QVariant getTableData(int row, int column);

  /**
   * @brief Setze Datensatz auf Objektname
   */
  void setValue(const QString &objName, const QVariant &value);

  /**
   * @brief Für Telefon Nummern eine extra Eingabe.
   * Grund hierfür ist, das die DIenstleister diese nicht Validieren!
   */
  void setPhone(const QString &objName, const QVariant &value);

  /**
   * @brief Nehme Datensatz von Objekt
   */
  const QVariant getValue(const QString &objName);

  /**
   * @brief Käuferinformationen auslesen!
   */
  const QHash<QString, QVariant> getCustomerData();
};
}; // namespace Antiqua

#endif // ANTIQUA_PURCHASEOVERVIEW_H
