// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERUPDATE_DIALOG_H
#define ANTIQUACRM_ORDERUPDATE_DIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY UpdateDialog : public QDialog {
  Q_OBJECT

private:
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
  explicit UpdateDialog(QWidget *parent = nullptr);
  int exec() override;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ORDERUPDATE_DIALOG_H
