// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERUPDATE_DIALOG_H
#define ANTIQUACRM_ORDERUPDATE_DIALOG_H

#include <AntiquaCRMPlugin>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

class ANTIQUACRM_LIBRARY BooklookerDialog final
    : public AntiquaCRM::UpdateDialog {
  Q_OBJECT

protected:
  QString p_order_id;
  QJsonObject p_system_data;
  QJsonObject p_post_data;

  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QRadioButton *m_rb7;
  QDialogButtonBox *m_buttonBar;

  void prepareAction() override;

protected Q_SLOTS:
  void networkAction() override;

public:
  explicit BooklookerDialog(QWidget *parent = nullptr);
  int exec(const QString &orderId, const QJsonObject &data) override;
};

#endif // ANTIQUACRM_ORDERUPDATE_DIALOG_H
