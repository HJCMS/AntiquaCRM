// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_CMDABEBOOKS_DOCUMENT_H
#define ANTIQUACMD_CMDABEBOOKS_DOCUMENT_H

#include <QByteArray>
#include <QDateTime>
#include <QDomDocument>
#include <QDomElement>
#include <QPair>
#include <QString>
#include <QTimeZone>

#ifndef ABEBOOKS_CHARSET
#define ABEBOOKS_CHARSET QByteArray("ISO-8859-1")
#endif

struct AbeBooksAccess {
  QString user;
  QString key;
};
Q_DECLARE_METATYPE(AbeBooksAccess);

class AbeBooksDocument final : public QDomDocument {
private:
  QString apiLogin;
  QString apiKey;

public:
  explicit AbeBooksDocument(
      const AbeBooksAccess &config,
      const QString &element = QString("orderUpdateRequest"));

  explicit AbeBooksDocument(const QDomDocument &other);

  /**
   * @brief Error Response Codes
   */
  QPair<int, QString> errorResponseCode();

  /**
   * @brief notExists
   * @return true - if Tag by Name not exists!
   */
  bool notExists(const QString &tag) const;

  /**
   * @brief fetchNodes
   * @return DomNodeList
   */
  const QDomNodeList fetchNodes(const QString &tag) const;

  /**
   * @brief firstChildNode
   * @return DomNode
   */
  const QDomNode firstChildNode(const QDomElement &parent,
                                const QString &tag) const;

  /**
   * @brief nodeIntValue
   * @return __int64
   */
  qint64 nodeIntValue(const QDomNode &node) const;

  /**
   * @brief createAction
   * This tag tells the Order Update server which action to take and which user
   * should execute the action. The tag has one parameter and two elements: The
   * parameter "name" may have the value of "getAllNewOrders", "getOrder",
   * "update" or "updateShipping", indicating which action to take. The value of
   * the elements "username" and "password" must represent a valid AbeBooks.com
   * seller, and this seller must be the owner of the purchase order being
   * processed by the action.
   * @code
   * <action name="$ac">
   *  <username>$USERNAME</username>
   *  <password>$APIKEY</password>
   * </action>
   * @endcode
   * @return action Element
   */
  const QDomElement createAction(const QString &tag);

  /**
   * @brief Reading purchaseMethod Node
   * Read the purchase method in use for the purchase order.
   * It may have the following values:
   *  @li SD - Seller Direct
   *  @li CC - AbeBooks processed credit card
   * Attribute "type" for Seller Direct orders (SD) only!
   *  @li Check
   *  @li PayPal
   *  @li Direct Debit (Personally Authorized Payment)
   *  @li Bank/Wire Transfer
   *  @li Cash on Delivery (COD)
   *  @li Money Order
   *  @li Bank Draft
   *  @li Invoice
   *  @li Cash
   * @code
   *  <purchaseMethod type="Invoice">SD</purchaseMethod>
   * @endcode
   * @return QPair<"TEXTNODE","TYPE">
   */
  const QPair<QString, QString> getPurchaseMethod();

  /**
   * @brief getPurchaseOrderList
   * The Purchase Order List Response (getAllNewOrders Request)
   * @return DomNodeList "purchaseOrder"
   */
  const QDomNodeList getPurchaseOrderList();

  /**
   * @brief getOrderDate
   * The order date of the purchase order item.
   * @param parent
   * @return DateTime
   */
  const QTimeZone fetchTimeZone(const QDomElement &orderNode);
  const QDateTime getOrderDate(const QDomElement &orderNode);

  /**
   * @brief purchaseOrderItem list
   * @param node = purchaseOrderNode
   * @return <purchaseOrderItem/> List
   */
  const QDomNodeList getOrderItemList(const QDomNode &node);

  /**
   * @brief Get TextNode::value() from Node
   * @param parent  Parent Node
   * @param tag   node Name
   */
  const QString getTagText(const QDomNode &parent, const QString &tag) const;

  /**
   * @brief getNodeValue
   */
  const QVariant getNodeValue(const QDomNode &parent);

  /**
   * @brief getAddressValues
   */
  const QVariant getAddressValue(const QDomNode &addressNode,
                                 const QString &tag);
  const QString getFullname(const QDomNode &addressNode);
  const QPair<QString, QString> getPerson(const QDomNode &addressNode);
  const QVariant getPostalCode(const QDomNode &addressNode);
  const QString getLocation(const QDomNode &addressNode);
  const QString getCountry(const QDomNode &addressNode);
  const QString getStreet(const QDomNode &addressNode);
  const QString getPhone(const QDomNode &addressNode);
  const QString getEMail(const QDomNode &addressNode);
};

#endif // ANTIQUACMD_CMDABEBOOKS_DOCUMENT_H
