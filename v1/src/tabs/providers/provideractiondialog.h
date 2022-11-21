// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERACTIONDIALOG_H
#define ANTIQUACRM_PROVIDERACTIONDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

class ProviderActionDialog final : public QDialog {
  Q_OBJECT

private:
  QRadioButton *m_rb1;
  QRadioButton *m_rb2;
  QRadioButton *m_rb3;
  QRadioButton *m_rb4;
  QRadioButton *m_rb5;
  QRadioButton *m_rb6;
  QRadioButton *m_rb7;
  QDialogButtonBox *m_buttonBar;

private Q_SLOTS:
  void prepareAction();

Q_SIGNALS:
  void sendPluginAction(const QJsonObject &obj);

public:
  explicit ProviderActionDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_PROVIDERACTIONDIALOG_H
