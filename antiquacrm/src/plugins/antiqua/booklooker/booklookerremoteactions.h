// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERREMOTEACTIONS_PLUGIN_H
#define BOOKLOOKERREMOTEACTIONS_PLUGIN_H

#include <QAction>
#include <QDialog>
#include <QObject>
#include <QToolBar>
#include <QWidget>

#include <AntiquaCRM>
#include <AntiquaInterface>

class ANTIQUACORE_EXPORT BooklookerRemoteActions final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString p_orderId;
  QToolBar *m_toolBar;
  QAction *ac_commit;
  QAction *ac_quit;
  QWidget *m_centralWidget;
  Antiqua::PaymentOrderUpdate *m_orderUpdate;

private Q_SLOTS:
  void updateOrderChanged(int);

Q_SIGNALS:
  void sendUpdateOrderStatus(const QString &orderId, const QString &status);

public:
  explicit BooklookerRemoteActions(QWidget *parent = nullptr);
  int exec(const QString &orderId);
};

#endif // BOOKLOOKERREMOTEACTIONS_PLUGIN_H
