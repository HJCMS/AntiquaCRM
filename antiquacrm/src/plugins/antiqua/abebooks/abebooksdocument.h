// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSDOCUMENT_PLUGIN_H
#define ABEBOOKSDOCUMENT_PLUGIN_H

#include <QDomDocument>
#include <QDomElement>
#include <QMetaType>
#include <QString>

#include <AntiquaCRM>
#include <AntiquaInterface>

struct ANTIQUACORE_EXPORT AbeBooksAccess {
  QString user;
  QString key;
};
Q_DECLARE_METATYPE(AbeBooksAccess);

class ANTIQUACORE_EXPORT AbeBooksDocument : public QDomDocument {
private:
  QString apiLogin;
  QString apiKey;

public:
  explicit AbeBooksDocument( const AbeBooksAccess &config,
      const QString &element = QString("orderUpdateRequest"));

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
};

#endif // ABEBOOKSDOCUMENT_PLUGIN_H
