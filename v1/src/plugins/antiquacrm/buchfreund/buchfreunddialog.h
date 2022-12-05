// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BUCHFREUND_DIALOG_H
#define ANTIQUACRM_BUCHFREUND_DIALOG_H

#include <AntiquaCRMPlugin>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class ANTIQUACRM_LIBRARY BuchfreundDialog final
    : public AntiquaCRM::UpdateDialog {
  Q_OBJECT

protected:
  QString p_order_id;
  QJsonObject p_system_data;
  QJsonObject p_post_data;

  bool prepareAction() override;

protected Q_SLOTS:
  void networkAction() override;

public Q_SLOTS:
  void statusMessage(const QString &) override {};

public:
  explicit BuchfreundDialog(QWidget *parent = nullptr);
  int exec(const QString &orderId, const QJsonObject &data) override;
};

#endif // ANTIQUACRM_BUCHFREUND_DIALOG_H
