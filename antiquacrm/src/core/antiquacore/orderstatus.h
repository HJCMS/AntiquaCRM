#ifndef ORDERSTATUS_ANTIQUACORE_H
#define ORDERSTATUS_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QList>
#include <QMetaType>
#include <QString>

/**
 * @brief SQL Order Status handle
 * @class OrderStatus
 */
class ANTIQUACORE_EXPORT OrderStatus {
public:
  explicit OrderStatus();
  OrderStatus(int index, const QString &title);

  /**
   * @brief set SQL Index
   */
  void setIndex(int index = -1);

  /**
   * @brief get SQL Index
   */
  int index();

  /**
   * @brief set SQL Title
   */
  void setTitle(const QString &title = QString());

  /**
   * @brief get SQL title
   */
  const QString title();

private:
  int p_index;
  QString p_title;
};
Q_DECLARE_TYPEINFO(OrderStatus, Q_PRIMITIVE_TYPE);

/**
 * @brief OrderStatus List class
 */
class ANTIQUACORE_EXPORT OrderStatusList : public QList<OrderStatus> {
private:
  /**
   * @brief filled by Constructor
   */
  QList<OrderStatus> p_list;

public:
  /**
   * @brief Initial and fills the OrderStatusList
   */
  explicit OrderStatusList();

  /**
   * @brief Current OrderStatusList Size
   */
  int size();

  /**
   * @brief find Trust Object with Index
   */
  const OrderStatus status(int index);

  /**
   * @brief find Trust::index() with Index
   */
  const int index(int index);

  /**
   * @brief find Trust::title() with Index
   */
  const QString title(int index);
};
Q_DECLARE_TYPEINFO(OrderStatusList, Q_COMPLEX_TYPE);

#endif // ORDERSTATUS_ANTIQUACORE_H
