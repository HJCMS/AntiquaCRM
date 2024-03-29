// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef COMPANY_COMPANYDATA_H
#define COMPANY_COMPANYDATA_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QStatusBar>
#include <QObject>
#include <QWidget>

class CompanyWidget;

/**
 * @ingroup _companydata
 * @brief The Company Settings Dialog
 * @class CompanyData
 */
class CompanyData : public QDialog {
  Q_OBJECT

private:
  CompanyWidget *m_editWidget;
  QDialogButtonBox *m_btnBox;
  QStatusBar *m_statusBar;

private Q_SLOTS:
  void closeDialog(bool);

public:
  explicit CompanyData(QWidget *parent = nullptr);
  int exec() override;
};

#endif // COMPANY_COMPANYDATA_H
